#include "libmx.h"

void mx_pop_front(t_list **head){
    if (head == NULL || *head == NULL){
            return;
    }
     t_list *temp = NULL;
    if ((*head)->next == NULL) {
            free(*head);
            *head = NULL;
        }else{
            temp = (*head)->next;
            free(*head);
            *head = temp;
        }
}


