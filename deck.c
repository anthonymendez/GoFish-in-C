#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "deck.h"

/*
 * Function: shuffle
 * -----------------
 * Initializes deck_instance and shuffles it.
 * Resets the deck if a prior game has been played.
 * 
 * Returns: 0 if no error, and non-zero on error
 */
int shuffle() {
    int i, j;

    /* Check if our deck of cards is empty */
    if(&deck_instance.list[0].suit == NULL || deck_instance.list[0].suit == '\0') {
        /* Fill up deck with cards */
        char suits[4] = {'C', 'D', 'H', 'S'};
        char ranks[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
        for(i = 0; i < 4; i++) {
            for(j = 0; j < 13; j++) {
                struct card new_card = {.suit=suits[i], .rank=ranks[j]};
                deck_instance.list[deck_instance.top_card++] = new_card; 
            }
        }

    /* We never completed the loop so something went wrong */
    if(i != 4 || j != 13)
        return -1;

    } else {
        deck_instance.top_card = 51;
    }

    /* Shuffle Deck 
     * Fisher-Yates Shuffle
     * https://en.wikipedia.org/wiki/Fisher-Yates_shuffle
     */
    srand(time(0));
    for(i = 51; i > 0; i--) {
        j = rand() % i;
        struct card temp = deck_instance.list[i];
        deck_instance.list[i] = deck_instance.list[j];
        deck_instance.list[j] = temp;
    }

    /* We never completed the loop so something went wrong */
    if(i != 0)
        return -1;

    return 0;
}

/*
 * Function: deal_player_cards
 * ---------------------------
 * Deal 7 random cards to the player specified in the functions.
 * Remove the dealt cards from the deck.
 *
 * target: pointer to the player to be dealt cards
 *
 * returns: 0 if no error, and non-zero on error
 */
int deal_player_cards(struct player* target) {
    int i;
    for(i = 0; i < 7; i++) {
       add_card(target, &(deck_instance.list[deck_instance.top_card--])); 
    }

    if(i != 7)
        return -1;

    return 0;
}

/*
 * Function: next_card
 * -------------------
 *  Return a pointer to the top card on the deck.
 *  Removes that card from the deck. 
 *
 *  returns: pointer to the top card on the deck.
 *  TODO: TEST
 */
struct card* next_card() {
    if(deck_instance.top_card < 0)
        return NULL;

    return &deck_instance.list[deck_instance.top_card--];
}
