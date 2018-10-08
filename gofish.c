#include <stdio.h>
#include "deck.h"

int main(int args, char* argv[]) {
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
