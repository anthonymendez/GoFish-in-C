#include <stdio.h>
#include "gofish.h"

int main(int args, char* argv[]) {
    fprintf(stdout, "Put your code here.");
}

void game_start() {
    shuffle();
    reset_player(&user);
    reset_player(&computer);
    deal_player_cards(&user);
    deal_player_cards(&computer);
}

void game_loop() {

}

void game_end() {
    char response;

    if(current == &user) {
        fprintf(stdout, "Player 1 Wins! 7-%d\n", 0);
    } else {
        fprintf(stdout, "Player 2 Wins! 7-%d\n", 0);
    }

    do {
        fprintf(stdout, "Do you want to play again [Y/N]: ");
        scanf("%s", &response);

        if(response == 'Y') {
            game_start();
            break;
        } else if(response == 'N') {
            fprintf(stdout, "Thank you for playing GoFish!\nExiting...\n");
            break;
        }
    }while(1);
}

char rand_rank_in_hand(struct player* target) {
    return '0';
}
