#ifndef DECK_H
#define DECK_H

#include <stdlib.h>
#include "player.h"
#include "card.h"

/*
 * Structure: deck
 * ---------------
 * An array of 52 cards.
 */
struct deck {
  struct card list[52];
  int top_card;
};

/*
 * Variable: deck_instance
 * -----------------------
 * 
 * Go Fish uses a single deck
 */
struct deck deck_instance;

/*
 * Function: shuffle
 * --------------------
 * Initializes deck_instance and shuffles it.
 * Resets the deck if a prior game has been played.
 * 
 * returns: 0 if no error, and non-zero on error
 */
int shuffle();

/*
 * Function: deal_player_cards
 * ---------------------------
 * Deal 7 random cards to the player specified in the function.
 * Remove the dealt cards from the deck.
 * 
 * target: pointer to the player to be dealt cards
 * 
 * returns: 0 if no error, and non-zero on error
 */
int deal_player_cards(struct player* target);

/*
 * Function: next_card
 * -------------------
 * Return a pointer to the top card on the deck.
 * Removes that card from the deck.
 * 
 * returns: pointer to the top card on the deck.
 */
struct card* next_card( );

/*
 * Function: size
 * --------------
 * Return the number of cards left in the current deck.
 * 
 * returns: number of cards left in the deck.
 */
size_t deck_size( );

#endif
