#include <stdio.h>
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
    /* Empty Hand Linked List */
    if(target->hand_size == 0) {
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
        if(i != target->hand_size-1)
            return -1;
        current->next = (struct hand*)malloc(sizeof(struct hand));
        current->next->top = *new_card;
        current->next->next = NULL;
    }
    target->hand_size++;

    /* Check if we have a book */
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
    /* If our hand_size is empty, we return -1 */
    if(target->hand_size <= 0)
        return -1;

    /* Go through and find our card from old_card 
     * If we find it, we set found = 1, and break
     * out of the loop.
     * If not, we go through the entire loop, and
     * return -1
     */
    int i;
    int found = 0;
    struct hand* current = target->card_list;
    struct hand* before = NULL;
    for(i = 0; i < target->hand_size; i++) {
        if(current != NULL &&
           (current->top.suit == old_card->suit && 
            current->top.rank == old_card->rank)) {
            found = 1;
            break;
        }
        before = current;
        current = current->next;
    }
    /* Card not found so we return -1 */
    if(found == 0) {
        return -1;
    }

    if(before == NULL || 
       &(before->top.suit) == NULL || 
       before->top.suit == '\0' ||
       before->top.suit == 0) {
        /* Handle removing card from beginning of the list */
        target->card_list = target->card_list->next;
        free(current);
        target->hand_size--;

        return 0;
    } else if(current == NULL || 
              &(current->top.suit) == NULL || 
              current->top.suit == '\0') {
        /* If we're getting NULL from our pointer,
         * char memory address or we're getting 0 
         * from our char, we return -1 as something 
         * went wrong.
         */
        return -1;
    } else {
        /* Handle removing card from middle of the list */
        before->next = current->next;
        free(current);
        target->hand_size--;

        return 0;
    }

    return -1;
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
 */
char check_add_book(struct player* target) {
    if(target->card_list == NULL) {
        return 0;
    }

    /* Get last card in list because we know that's the only one with the possibility of being a book */
    struct hand* last_hand = target->card_list;
    int i = 0;
    while(i++ < target->hand_size && last_hand->next != NULL){
        last_hand = last_hand->next;
    }
    int count = 1;
    struct card last_card = last_hand->top;
    /* Contains the other hands where we have a card of the same rank as last_card */
    struct hand* first_hand;
    struct hand* second_hand;
    struct hand* third_hand;
    struct hand* current = target->card_list;
    for(i = 0; i < target->hand_size - 1 && current != NULL; i++) {
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
        current = current->next;
    }
    /* We count is 4, then we have 4 cards of the same rank 
     * We remove the cards from the players hand, add the
     * rank to their book, and return the rank.
     */
    if(count == 4) {
        char rank = first_hand->top.rank;

        for(i = 0; i < 7; i++) {
            if(target->book[i] == 0 || target->book[i] == '\0') {
                target->book[i] = rank;
                break;
            }
        }

        remove_card(target, &first_hand->top);
        remove_card(target, &second_hand->top);
        remove_card(target, &third_hand->top);
        remove_card(target, &last_hand->top);

        return rank;
    }

    return 0;
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
 */
int search(struct player* target, char rank) {
    int i;
    struct hand* current_hand = target->card_list;
    for(i = 0; i < target->hand_size && current_hand != NULL; i++) {
        if(current_hand->top.rank == rank)
            return 1;

        current_hand = current_hand->next;
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
 */   
int transfer_cards(struct player* src, struct player* dest, char rank) {
    int i;
    int transferred = 0;
    struct hand* src_hand = src->card_list;
    /* Go through, add all matching cards to dest hand,
     * remove from src hand, set src hand to next hand,
     * and increment transferred any matching cards
     */
    for(i = 0; src_hand != NULL; i++) {
        if(src_hand->top.rank == rank) {
            add_card(dest, &src_hand->top);
            remove_card(src, &src_hand->top);
            src_hand = src_hand->next;
            transferred++;
        } else {
            src_hand = src_hand->next;
        }
    }

    if(i != src->hand_size) {
        return -1;
    }

    return transferred;
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
 */
int game_over(struct player* target) {
    if(target == NULL)
        return -1;

    int i;
    for(i = 0; i < 7; i++) {
        if(target->book[i] == '\0' || target->book[i] == 0)
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

    fprintf(stdout, "(debug)handsize after reset: %d\n", target->hand_size);
    if(i != 7 || target->hand_size != 0) {
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
 */
char computer_play(struct player* target) {
    int i;
    int randIndex = rand() % target->hand_size;
    struct hand* current_hand = target->card_list;
    for(i = 0; i < randIndex; i++) {
        current_hand = current_hand->next;
    }

    fprintf(stdout, "Player 2's turn, enter a Rank: %c\n", current_hand->top.rank);
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
 */
char user_play(struct player* target) {
    char rank;
    do {
        fprintf(stdout, "Player 1's turn, enter a Rank: ");
        char buf[4] = "";
        scanf("%3s", buf); /* If {Enter} is pressed before other input, this still blocks. I believe this is fine. */
        while(getchar() != '\n'); /* Clear anything after the 3rd character from stdin */

        /* Check for a "10" */
        if(buf[0] == '1' && buf[1] == '0' && buf[2] == '\0')
            rank = 'T';
        else if(buf[1] == '\0')
            rank = buf[0];
        else { /* Invalid input length */
            fprintf(stdout, "Error - must have at least one card from rank to play\n");
            continue;
        }

        /* If the selected rank is in the player's hand, return it */
        if(search(target, rank) && buf[0] != 'T') /* Input 'T' improperly results in a successful search */
            break;

        fprintf(stdout, "Error - must have at least one card from rank to play\n");
    }while(1);

    return rank;
}
