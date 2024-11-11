#include "../inc/libmx.h"

void* mx_list_element_at(t_list* list, int idx) {
	while (idx-- > 0) {
		if (!list)return NULL;
		list = list->next;
	}

	return list->data;
}