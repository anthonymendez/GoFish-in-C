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
}

int main(int args, char* argv[]) {
    srand(time(NULL));
    
    /* TODO: Remove after testing! */
    test();
    return;
    
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
    fprintf(stdout, "(debug)###BEGIN game_start###\n");
    reset_player(&user);
    reset_player(&computer);
    
    shuffle();
    deal_player_cards(&user);
    deal_player_cards(&computer);
    
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
    fprintf(stdout, "(debug)###BEGIN game_loop###\n");
    /* Print ex "Player 1's Hand - 5S 3H JC 9D 9H AS QC 7D" */
    /* Print ex "Player 1's Book - 3 2 J A 6 Q" */
    /* Print ex "Player 2's Book - 9 5 10 4 5 K" */
    char r;
    if(current == &user) {
        r = user_play(current);
    } else { /* Computer's turn */
        r = computer_play(current);
    }
    
    if(search(next_player, r) == 0) { /* Go fish, next player's turn */
        //fprintf(stdout, "Player %d has no \n");
        //fprintf(stdout, "Go Fish, Player %d draws a card\n"); /* TODO: Separate with 'if' for user/computer */
        //fprintf(stdout, "Player %d's turn", ((current == &user) ? 1 : 2));
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
    fprintf(stdout, "(debug)###BEGIN game_end###\n");
    /* Count books of loser */
    int count = 0;
    while(next_player->book[count] != 0 && count < 6) { /* Last slot (index 6) will always be empty since they lost */
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
    if(target->hand_size == 0)
        return;

    struct hand* current = target->card_list;
    fprintf(stdout, "%s%c", pR(current->top.rank), current->top.suit);

    int i;
    for(i = 1; i < target->hand_size; i++) {
        current = current->next;
        fprintf(stdout, " %s%c", pR(current->top.rank), current->top.suit);
    }
    
    fprintf(stdout, "\n");
}

/* TODO: Document! Including in header! */
void print_book(struct player* target) {
    if(target == NULL || target->book == NULL || target->book[0] == '\0' || target->book[0] == 0)
        return;

    fprintf(stdout, "%s", pR(target->book[0]));

    int i = 1;
    while(i < 7 && target->book[i] != '\0' && target->book[i] != 0) {
        fprintf(stdout, " %s", pR(target->book[i++]));
    }
    
    fprintf(stdout, "\n");
}