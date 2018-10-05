#ifndef CARD_H
#define CARD_H

/*
  Valid suits: C, D, H, and S
  Valid ranks: 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, A
*/
struct card
{
  char suit;
  char rank[2];
};

/*
  Linked list of cards in hand.
    top: first card in hand
    next: pointer to next card in hand
*/
struct hand
{
  struct card top;
  struct hand* next;
};

#endif
