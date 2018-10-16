#include <stdio.h>
#include <time.h>
#include "gofish.h"

int main(int args, char* argv[]) {
    srand(time(NULL));

    do {
        game_start();
        do {
            /* Play a round */
            if(game_loop())
                break; /* If there is a winner, go to game_end */
            current = next_player;
        } while(1);
    } while(game_end());
    fprintf(stdout, "Exiting\n");
    return 0;
}

/*
 * Function: game_start
 * --------------------
 * Called at the start of a new game.
 * Shuffles the deck, resets instances of players,
 * deals cards to each player, and sets the human player
 * as the current player. 
 */
void game_start() {
    reset_player(&user);
    reset_player(&computer);

    fprintf(stdout, "Shuffling deck...\n");
    shuffle();
    deal_player_cards(&user);
    deal_player_cards(&computer);

    /* It is possible to be dealt, at most, one book */
    if(user.book[0] != 0)
        fprintf(stdout, "Player 1 books %s", pR(user.book[0]));
    if(computer.book[0] != 0)
        fprintf(stdout, "Player 2 books %s", pR(computer.book[0]));

    current = &user;
    next_player = &computer;
}

/*
 * Function: game_loop
 * -------------------
 * Called after game_start.
 * For more info, read notes.txt
 * 
 * Return: 1 if there is a winner, 0 otherwise
 */
int game_loop() {
    fprintf(stdout, "\n");

    /* Print hand and book statuses */
    fprintf(stdout, "Player 1's Hand - ");
    print_hand(&user);
    fprintf(stdout, "Player 1's Book - ");
    print_book(&user);
    fprintf(stdout, "Player 2's Book - ");
    print_book(&computer);

    struct player* other_player = (current == &user) ? &computer : &user;

    if(game_over(current)) {
        return 1;
    }
    if(game_over(other_player)) { /* Shouldn't happen */
        current = other_player; /* Signify the correct winner */
        return 1;
    }

    /*
    "If a player runs out of cards, then they have to draw a card on their next turn.
    It does not end the game." -Irwin (piazza)
    */
    char r;
    if(current->hand_size > 0) { /* Non-empty hand */
        /* Get rank guess input */
        if(current == &user) { /* User's turn */
            r = user_play(current);
        } else { /* Computer's turn */
            r = computer_play(current);
        }
    } else /* Empty hand */
        r = 'X'; /* Invalid rank, so search will always fail and the player will Go Fish */

    /* Handle input */
    if(search(other_player, r) == 0) { /* Go Fish */
        if(r != 'X') /* Non-empty hand */
            fprintf(stdout, "    - Player %d has no %s's\n", ((current == &user) ? 2 : 1), pR(r));

        struct card* fished_card = next_card();
        int next_book_i = 0;
        if (fished_card != NULL) {
            if(current == &user)
                fprintf(stdout, "    - Go Fish, Player 1 draws %s%c\n", pR(fished_card->rank), fished_card->suit);
            else {
                if (fished_card->rank == r) {
                    fprintf(stdout, "    - Go Fish, Player 2 draws %s%c\n", pR(fished_card->rank), fished_card->suit);
                }
                else {
                    fprintf(stdout, "    - Go Fish, Player 2 draws a card\n");
                }
            }

           while(current->book[next_book_i] != 0) {
                next_book_i++;
            }
            add_card(current, fished_card);
            if(current->book[next_book_i] != 0)
                fprintf(stdout, "    - Player %d books %s\n", ((current == &user) ? 1 : 2), pR(fished_card->rank));
        }
        /* If a book was added or the asked rank was drawn, play again */
        if(fished_card != NULL && (
            current->book[next_book_i] != 0 || 
            fished_card->rank == r)
          ) {
            next_player = current;
            fprintf(stdout, "    - Player %d gets another turn\n", ((current == &user) ? 1 : 2));
        } else { /* Otherwise, switch players' turns */
            next_player = other_player;
            fprintf(stdout, "    - Player %d's turn\n", ((next_player == &user) ? 1 : 2));
        }
    } else { /* Transfer cards, play again */
        /* Print the cards of the guessed rank that each player has */
        struct player* print_player = current;
        int i;
        for(i = 0; i < 2; i++) {
            if (current == &computer && print_player == &computer) {
                print_player = (current == &user) ? &computer : &user; /* Switch to other player */
                continue;
            }
            fprintf(stdout, "    - Player %d has ", ((print_player == &user) ? 1 : 2));
            int j;
            struct hand* h = print_player->card_list;
            int rank_count = 0;
            for(j = 0; j < print_player->hand_size && h != NULL; j++) {
                if(h->top.rank == r) {
                    if(rank_count++ > 0)
                        fprintf(stdout, ", ");
                    fprintf(stdout, "%s%c", pR(r), h->top.suit);
                }

                h = h->next;
            }
            fprintf(stdout, "\n");
            print_player = (current == &user) ? &computer : &user; /* Switch to other player */
        }

        int next_book_i = 0;
        while(current->book[next_book_i] != 0) {
            next_book_i++;
        }
        transfer_cards(other_player, current, r);
        if(current->book[next_book_i] != 0)
            fprintf(stdout, "    - Player %d books %s\n", ((current == &user) ? 1 : 2), pR(r));

        /* If a book was added, play again */
        if(current->book[next_book_i] != 0) {
            next_player = current;
            fprintf(stdout, "    - Player %d gets another turn\n", ((current == &user) ? 1 : 2));
        } else { /* Otherwise, switch players' turns */
            next_player = other_player;
            fprintf(stdout, "    - Player %d's turn\n", ((next_player == &user) ? 1 : 2));
        }
    }
    return 0;
}

/*
 * Function: game_end
 * ------------------
 * Called after someone wins in 
 * GoFish from game_loop.
 * Declares the winner and asks the human
 * if s/he wants to play again.
 * If Y is entered, go to game_start.
 * Else if N is entered, end game and close program.
 * 
 * Return: 1 to play again, 0 to exit
 */
int game_end() {
    struct player* other_player = (current == &user) ? &computer : &user;
    
    /* Count books of loser */
    int count = 0;
    while(other_player->book[count] != 0 && count < 6) { /* Last slot (index 6) will always be empty since they lost */
        count++;
    }
    if(current == &user) { /* User won :D */
        fprintf(stdout, "Player 1 Wins! 7-%d\n", count);
    } else {
        fprintf(stdout, "Player 2 Wins! 7-%d\n", count);
    }

    char yn[3] = "";
    int tryAgain = 0;
    do {
        if(tryAgain) {
            fprintf(stdout, "Error - must enter \"Y\" or \"N\"");
        }
        fprintf(stdout, "\nDo you want to play again [Y/N]: ");
        scanf("%2s", &yn);
        tryAgain = 1;

        while(getchar() != '\n');

        if(yn[1] != '\0')
            continue;

        if(yn[0] == 'Y')
            return 1;
        else if(yn[0] == 'N')
            return 0;
        else
            continue;
    }while(1);
}

/* TODO: Document! Including in header! */
const char* pR(char r) { /* printableRank */
    if(r == 'T') {
        static char ten[] = "10";
        return ten;
    }
    static char rS[2];
    rS[0] = r;
    rS[1] = '\0';
    return rS;
}

/* TODO: Document! Including in header! */
void print_hand(struct player* target) {
    if(target->hand_size == 0) {
        fprintf(stdout, "\n");
        return;
    }

    struct hand* h = target->card_list;
    fprintf(stdout, "%s%c", pR(h->top.rank), h->top.suit);

    int i;
    for(i = 1; i < target->hand_size; i++) {
        h = h->next;
        fprintf(stdout, " %s%c", pR(h->top.rank), h->top.suit);
    }

    fprintf(stdout, "\n");
}

/* TODO: Document! Including in header! */
void print_book(struct player* target) {
    if(target == NULL || target->book == NULL || target->book[0] == '\0' || target->book[0] == 0) {
        fprintf(stdout, "\n");
        return;
    }

    fprintf(stdout, "%s", pR(target->book[0]));

    int i = 1;
    while(i < 7 && target->book[i] != '\0' && target->book[i] != 0) {
        fprintf(stdout, " %s", pR(target->book[i++]));
    }

    fprintf(stdout, "\n");
}
