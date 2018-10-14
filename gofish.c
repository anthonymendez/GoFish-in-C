#include <stdio.h>
#include <time.h>
#include "gofish.h"

/* TODO: Remove after testing! */
void test() {
    game_start();
    current->book[0] = '3';
    current->book[1] = 'T';
    current->book[2] = 'Q';
    print_hand(current);
    print_book(current);
    fprintf(stdout, "book[3] (between --'s): --%c-- (%d)\n", current->book[3], current->book[3] == 0);
    fprintf(stdout, "userplay:\n");
    char upo = user_play(current);
    fprintf(stdout, "userplay output (between --'s): --%c--\n", upo);
}

int main(int args, char* argv[]) {
    srand(time(NULL));

    /* TODO: Remove after testing! */
    //test();
    //return 0;

    do {
        game_start();
        do {
            game_loop(); /* Play a round */
            if(game_over(current))
                break;
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
 */
void game_loop() {
    fprintf(stdout, "\n");

    /* Print hand and book statuses */
    fprintf(stdout, "Player 1's Hand - ");
    print_hand(&user);
    fprintf(stdout, "(debug)Player 2's Hand - ");
    print_hand(&computer);
    fprintf(stdout, "Player 1's Book - ");
    print_book(&user);
    fprintf(stdout, "Player 2's Book - ");
    print_book(&computer);

    struct player* other_player = (current == &user) ? &computer : &user;

    /* TODO:
    "If a player runs out of cards, then they have to draw a card on their next turn.
    It does not end the game." -Irwin (piazza)
    So, here, if hand size is 0:
        Draw a card
        next_player = other_player;
        return
        Printout???
    */

    /* Get rank guess input */
    char r;
    if(current == &user) { /* User's turn */
        r = user_play(current);
    } else { /* Computer's turn */
        r = computer_play(current);
    }

    /* Handle input */
    if(search(other_player, r) == 0) { /* Go Fish */
        fprintf(stdout, "    - Player %d has no %s's\n", ((current == &user) ? 2 : 1), pR(r));
        struct card* fished_card = next_card(); /* TODO: Handle empty deck (here and elsewhere) */
        if(current == &user)
            fprintf(stdout, "    - Go Fish, Player 1 draws %s%c\n", pR(fished_card->rank), fished_card->suit);
        else
            fprintf(stdout, "    - Go Fish, Player 2 draws a card\n");

        int next_book_i = 0;
        while(current->book[next_book_i] != 0) {
            next_book_i++;
        }
        add_card(current, fished_card);
        if(current->book[next_book_i] != 0)
            fprintf(stdout, "    - Player %d books %s\n", ((current == &user) ? 1 : 2), pR(fished_card->rank));

        /* If a book was added or the asked rank was drawn, play again */
        if(current->book[next_book_i] != 0 || fished_card->rank == r) {
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
    /* TODO: Make sure printouts are exactly correct */

    fprintf(stdout, "Do you want to play again [Y/N]: ");
    char yn;
    scanf("%s", &yn);
    if(yn == 'Y') {
        return 1;
    }
    /* TODO: Make sure yn == 'N', otherwise prompt again? */
    return 0;
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