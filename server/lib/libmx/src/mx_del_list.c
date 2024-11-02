#include "../inc/libmx.h"

void mx_del_list(t_list* list, int len) {
    for (int i = 0; i < len; i++)
        mx_pop_back(&list);
}