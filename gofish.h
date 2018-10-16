#ifndef GOFISH_H
#define GOFISH_H

#include <stdlib.h>
#include "player.h"
#include "deck.h"

/*
   Define any prototype functions
   for gofish.h here.
*/

/*
 * Function: game_start
 * --------------------
 * Called at the start of a new game.
 * Shuffles the deck, resets instances of players,
 * deals cards to each player, and sets the human player
 * as the current player. 
 */
void game_start();

/*
 * Function: game_loop
 * -------------------
 * Called after game_start.
 * For more info, read notes.txt
 * 
 * Return: 1 if there is a winner, 0 otherwise
 */
int game_loop();

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
int game_end();

/*
 * Function: pR
 * ------------
 * The name is short for printableRank
 * Formats a rank for output.
 * Specifically, 'T' returns "10"
 * and all other input is "unchanged".
 * 
 * Return: String representing rank r
 */
const char* pR(char r);

/*
 * Function: print_hand
 * --------------------
 * Prints space-separated 2-character
 * representations of the cards in
 * player target's hand followed by a
 * newline.
 * The rank character precedes the
 * suit character.
 */
void print_hand(struct player* target);

/*
 * Function: print_book
 * --------------------
 * Prints space-separated
 * representations of the ranks of the
 * books in player target's hand
 * followed by a newline.
 */
void print_book(struct player* target);

/*
 * Instance Variables: current, next_player
 *
 * Points to the current player and the next player's turn
 */
struct player* current;
struct player* next_player;

#endif
