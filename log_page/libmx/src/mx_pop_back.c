#include "libmx.h"

void mx_pop_back(t_list **head){
   if (head == NULL || *head == NULL) {
        return;
    }

    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }

        t_list *list_free = *head;
        while (list_free -> next -> next){
            list_free = list_free -> next;
        }
        free(list_free -> next);
        list_free -> next = NULL;
        list_free = NULL;
}

