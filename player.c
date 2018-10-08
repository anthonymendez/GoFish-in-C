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
    /* Make sure our hand is empty */
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

    return 0;
}
