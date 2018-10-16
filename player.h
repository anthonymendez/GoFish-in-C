#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "card.h"

/*
 * Structure: player
 * -----------------
 *  Each player holds some number of cards in their hand
 *  Each player can have at most 7 "books" before winning
 */
struct player {
  struct hand* card_list;
  char book[7];
  size_t hand_size;
};

/* 
 * Instance Variables: user, computer
 * ----------------------------------
 * 
 * We only support 2 users: a human and a computer
 */
struct player user;
struct player computer;

/*
 * Function: add_card
 * -------------------
 * Add a new card to the player's hand.
 * 
 * target: the target player
 * new_card: pointer to the new card to add
 * 
 * returns: return 0 if no error, non-zero otherwise
 */
int add_card(struct player* target, struct card* new_card);

/*
 * Function: remove_card
 * ---------------------
 * Remove a card from the player's hand.
 * 
 * target: the target player
 * old_card: pointer to the old card to remove
 * 
 * returns: return 0 if no error, non-zero otherwise
 */
int remove_card(struct player* target, struct card* old_card);

/*
 * Function: check_add_book
 * ------------------------
 * Check if a player has all 4 cards of the same rank.
 * If so, remove those cards from the hand, and add the rank to the book.
 * Returns after finding one matching set of 4, so should be called after adding each a new card.
 * 
 * target: pointer to the player to check
 * 
 * Return: a char that indicates the book that was added; return 0 if no book added.
 */
char check_add_book(struct player* target);

/*
 * Function: search
 * ----------------
 * Search a player's hand for a requested rank.
 * 
 * rank: the rank to search for
 * target: the player (and their hand) to search
 * 
 * Return: If the player has a card of that rank, return 1, else return 0
 */
int search(struct player* target, char rank);

/*
 * Function: transfer_cards
 * ------------------------
 * Transfer cards of a given rank from the source player's 
 * hand to the destination player's hand. Remove transferred 
 * cards from the source player's hand. Add transferred cards 
 * to the destination player's hand.
 * 
 * src: a pointer to the source player
 * dest: a pointer to the destination player
 * rank: the rank to transfer
 * 
 * Return: 0 if no cards found/transferred, <0 if error, otherwise 
 * return value indicates number of cards transferred.
 */
int transfer_cards(struct player* src, struct player* dest, char rank);

/*
 * Function: game_over
 * -------------------
 * Boolean function to check if a player has 7 books yet 
 * and the game is over
 * 
 * target: the player to check
 * 
 * Return: 1 if game is over, 0 if game is not over
 */
int game_over(struct player* target);

/*
 * Function: reset_player
 * ----------------------
 * 
 * Reset player by free'ing any memory of cards remaining in hand,
 * and re-initializes the book.  Used when playing a new game.
 * 
 * target: player to reset
 * 
 * Return: 0 if no error, and non-zero on error.
 */
int reset_player(struct player* target);

/* 
 * Function: computer_play
 * -----------------------
 * 
 * Select a rank randomly to play this turn. The player must have at least
 * one card of the selected rank in their hand.
 * 
 * target: the player's hand to select from
 * 
 * Rank: return a valid selected rank
 */
char computer_play(struct player* target);

/*
 * Function: user_play
 * -------------------
 * 
 * Read standard input to get rank user wishes to play.  Must perform error 
 * checking to make sure at least one card in the player's hand is of the 
 * requested rank.  If not, print out "Error - must have at least one card from rank to play" 
 * and then re-prompt the user.
 * 
 * target: the player's hand to check
 * 
 * returns: return a valid selected rank
 */
char user_play(struct player* target);

#endif
