#include "libmx.h"


t_list *mx_sort_list(t_list *list, bool(*cmp)(void *, void *)) {
    if (list == NULL || cmp == NULL) {
        return list;
    }

    for (t_list *current = list; current; current = current->next) {
        for (t_list *nextNode = list; nextNode->next; nextNode = nextNode->next) {
            if (cmp(nextNode->data, nextNode->next->data)) {
                void *temp = nextNode->data;
                nextNode->data = nextNode->next->data;
                nextNode->next->data = temp;
            }
        }
    }
    return list;
}



