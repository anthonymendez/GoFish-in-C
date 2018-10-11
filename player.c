#include <stdio.h>
#include <time.h>
#include "player.h"

/*
 * Function: add_card
 * -------------------
 *  Add a new card to the player's hand. 
 *
 *  target: the target player
 *  new_card: pointer to the new card to add
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int add_card(struct player* target, struct card* new_card) {
    /* Make sure our hand is empty */ /* TODO: What is this comment to the left? */
    if(target->hand_size >= 7)
        return -1;

    /* Empty Hand Linked List */
    if(target->hand_size == 0) {
        target->hand_size++;
        target->card_list = (struct hand*)malloc(sizeof(struct hand));
        target->card_list->top = *new_card;
        target->card_list->next = NULL;
    } else {
    /* Go to node before first NULL pointer */
        int i;
        struct hand* current = target->card_list;
        for(i = 0; i < target->hand_size-1; i++) {
            current = current->next;
        }
        current->next = (struct hand*)malloc(sizeof(struct hand));
        current->next->top = *new_card;
        current->next->next = NULL;
        target->hand_size++;
    }

    /* Check if we have a book */
    /* TODO: Check if this belongs here (it almost certainly does) */
    check_add_book(target);

    return 0;
}

/*
 * Function: remove_card
 * ---------------------
 *  Remove a card from the player's hand. 
 *
 *  target: the target player
 *  old_card: pointer to the old card to remove
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int remove_card(struct player* target, struct card* old_card) {
    /* TODO: Comments for this function */
    if(target->hand_size <= 0)
        return -1;

    int i;
    struct hand* current = target->card_list;
    struct hand* before = NULL;
    for(i = 0; i < target->hand_size; i++) {
        if(current != NULL ||
           (current->top.suit == old_card->suit && 
            current->top.rank == old_card->rank))
            break;
        before = current;
        current = current->next;
    }
    if(before == NULL || 
       &(before->top.suit) == NULL || 
       before->top.suit == '\0') {
        target->card_list = target->card_list->next;
        free(current);
        target->hand_size--;
    } else if(current == NULL || 
              &(current->top.suit) == NULL || 
              current->top.suit == '\0') {
        return -1;
    } else {
        before->next = current->next;
        free(current);
        target->hand_size--;
    }

    return 0;
}

/*
 * Function: check_add_book
 * ------------------------
 *  Check if a player has all 4 cards of the same rank.
 *  If so, remove those cards from the hand, and add the rank to the book.
 *  Returns after finding one matching set of 4, so should be called after adding each a new card.
 * 
 *  target: pointer to the player to check
 *  
 *  Return: a char that indicates the book that was added; return 0 if no book added.
 * TODO: TEST
 */
char check_add_book(struct player* target) {
    if(target->card_list == NULL) {
        return 0;
    }

    /* Get last card in list because we know that's the only one with the possibility of being a book */
    struct hand* last_hand = target->card_list;
    int i = 0;
    while(i++ < target->hand_size && last_hand->next == NULL){
        last_hand = last_hand->next;
    }

    /* Something went wrong when looping */
    if(i != target->hand_size) {
        return 0;
    }

    int count = 1;
    struct card last_card = last_hand->top;
    /* Contains the other hands where we have a card of the same rank as last_card */
    struct hand* first_hand;
    struct hand* second_hand;
    struct hand* third_hand;
    struct hand* current = target->card_list;

    for(i = 0; i < target->hand_size - 1; i++) {
        if(current->top.rank == last_card.rank) {
            if(count == 1) {
                first_hand = current;
            } else if(count == 2) {
                second_hand = current;
            } else if(count == 3) {
                third_hand = current;
                count++;
                break;
            } else {
                /* Something went wrong */
                return 0;
            }
            count++;
        }
    }

    char rank = first_hand->top.rank;

    if(count == 4) {
        remove_card(target, &first_hand->top);
        remove_card(target, &second_hand->top);
        remove_card(target, &third_hand->top);
        remove_card(target, &last_hand->top);
    }

    return rank;
}

/*
 * Function: search
 * ----------------
 *  Search a player's hand for a requested rank.
 *  
 *  rank: the rank to search for
 *  target: the player (and their hand) to search
 *
 *  Return: If the player has a card of that rank, return 1, else return 0
 * TODO: TEST
 */
int search(struct player* target, char rank) {
    int i;
    struct hand* current_hand = target->card_list;
    for(i = 0; i < target->hand_size && current_hand != NULL; i++) {
        if(current_hand->top.rank == rank)
            return 1;
    }

    return 0;
}

/*
 * Function: transfer_cards
 * ------------------------
 *   Transfer cards of a given rank from the source player's 
 *   hand to the destination player's hand. Remove transferred
 *   cards from the source player's hand. Add transferred cards
 *   to the destination player's hand.
 *   
 *   src: a pointer to the source player
 *   dest: a pointer to the destination player
 *   rank: the rank to transfer
 *
 *   Return: 0 if no cards found/transferred, <0 if error, otherwise 
 *   return value indicates number of cards transferred
 *   TODO: TEST
 */   
int transfer_cards(struct player* src, struct player* dest, char rank) {
    int i;
    struct hand* src_hand = src->card_list;
    for(i = 0; i < src->hand_size && src_hand != NULL; i++) {
        if(src_hand->top.rank == rank) {
            add_card(src, &src_hand->top);
            src_hand = src_hand->next;
            remove_card(src, &src_hand->top);
        } else {
            src_hand = src_hand->next;
        }
    }

    if(i != src->hand_size) {
        return -1;
    }
    
    /* TODO (bugfix): Actually transfer the cards instead of just deleting them */

    return 0;
}

/*
 * Function: game_over
 * -------------------
 *   Boolean function to check if a player has 7 books yet
 *   and the game is over
 *
 *   target: the player to check
 *   
 *   Return: 1 if game is over, 0 if game is not over
 *   TODO: TEST
 */
int game_over(struct player* target) {
    if(target == NULL)
        return -1;

    int i;
    for(i = 0; i < 7; i++) {
        if(target->book[i] == 0)
            return 0;
    }

    /* Something went wrong with looping */
    if(i != 7) {
        return -1;
    }

    return 1;
}

/* 
 * Function: reset_player
 * ----------------------
 *
 *   Reset player by free'ing any memory of cards remaining in hand,
 *   and re-initializes the book.  Used when playing a new game.
 * 
 *   target: player to reset
 * 
 *   Return: 0 if no error, and non-zero on error.
 *   TODO: TEST
 */
int reset_player(struct player* target) {
    /* Free card_list */
    while(target->card_list != NULL) {
        remove_card(target, &target->card_list->top);
    }

    if(target->hand_size != 0 || target->card_list != NULL) {
        return -1;
    }

    /* Set book elements to 0 */
    int i;
    for(i = 0; i < 7; i++) {
        target->book[i] = 0;
    }

    if(i != 7) {
        return -1;
    }

    /* Set hand_size to 0 */
    target->hand_size = 0;

    if(target->hand_size != 0) {
        return -1;
    }

    return 0;
}

/* 
 * Function: computer_play
 * -----------------------
 *
 *   Select a rank randomly to play this turn. The player must have at least
 *   one card of the selected rank in their hand.
 *
 *   target: the player's hand to select from
 *
 *   Rank: return a valid selected rank
 *   TODO: TEST
 */
char computer_play(struct player* target) {
    int i;
    srand(time(0));
    int randIndex = rand() % target->hand_size;
    struct hand* current_hand = target->card_list;
    for(i = 0; i <= randIndex; i++) {
        current_hand = current_hand->next;
    }

    return current_hand->top.rank;
}

/* 
 * Function: user_play
 * -------------------
 *
 *   Read standard input to get rank user wishes to play.  Must perform error
 *   checking to make sure at least one card in the player's hand is of the 
 *   requested rank.  If not, print out "Error - must have at least one card from rank to play"
 *   and then re-prompt the user.
 *
 *   target: the player's hand to check
 * 
 *   returns: return a valid selected rank
 *   TODO: TEST
 */
char user_play(struct player* target) {
    char rank;
    do {
        fprintf(stdout, "Player 1's turn, enter a Rank: ");
        scanf("%s", &rank);

        if(search(target, rank))
            break;

        fprintf(stdout, "Error - must have at least one card from rank to play\n");
    }while(1);

    return rank;
}
