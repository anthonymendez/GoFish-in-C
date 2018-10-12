#include <stdio.h>
#include "gofish.h"

/* TODO: Remove after testing */
int testMain(int args, char* argv[]) {
    int i,j;
    shuffle();
    for(i = 0; i < 52; i++) {
        fprintf(stdout, "%c%c,", deck_instance.list[i].suit, deck_instance.list[i].rank);
    }
    fprintf(stdout, "\n");
    shuffle();
    for(i = 0; i < 52; i++) {
        fprintf(stdout, "%c%c,", deck_instance.list[i].suit, deck_instance.list[i].rank);
    }

    add_card(&user, &(deck_instance.list[0]));
    fprintf(stdout, "\n%c%c\n", user.card_list->top.suit, user.card_list->top.rank);
    add_card(&user, &(deck_instance.list[1]));
    fprintf(stdout, "%c%c\n", user.card_list->next->top.suit, user.card_list->next->top.rank);
    add_card(&user, &(deck_instance.list[2]));
    fprintf(stdout, "%c%c\n", user.card_list->next->next->top.suit, user.card_list->next->next->top.rank);
    add_card(&user, &(deck_instance.list[3]));
    fprintf(stdout, "%c%c\n", user.card_list->next->next->next->top.suit, user.card_list->next->next->next->top.rank);
    remove_card(&user, &(deck_instance.list[0]));
    fprintf(stdout, "New first card: %c%c\n", user.card_list->top.suit, user.card_list->top.rank);
    remove_card(&user, &(deck_instance.list[2]));
    fprintf(stdout, "2nd: %c%c\n", user.card_list->next->top.suit, user.card_list->next->top.rank);
    remove_card(&user, &(deck_instance.list[7]));
    fprintf(stdout, "7 removed or not\n");
    remove_card(&user, &(deck_instance.list[1]));
    fprintf(stdout, "1 removed or not\n");
    remove_card(&user, &(deck_instance.list[3]));
    fprintf(stdout, "3 removed or not\n");
    deal_player_cards(&user);
    fprintf(stdout, "cards dealt\n");
    deal_player_cards(&computer);
}

int main(int args, char* argv[]) {
    //return testMain(args, argv); /* TODO: Remove after testing */
    do {
        game_start();
        do {
            game_loop(); /* Play a round */
        } while(!game_over(current));
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
    static char ten[] = "10";
    if(r == 'T') {
        return ten;
    } else {
        static char rS[2];
        rS[0] = r;
        rS[1] = '\0';
        return rS;
    }
}