#include "../../inc/server.h"

t_group* group_from_data_list(t_list* list)
{
	if (mx_list_size(list) < 5)return NULL;

	t_group* ret = (t_group*)malloc(sizeof(t_group));

	ret->id = mx_atoi((char*)list->data);

	list = list->next;
	ret->name = (char*)list->data;

	list = list->next;
	ret->created_by = mx_atoi((char*)list->data);

	list = list->next;
	ret->creator_username = (char*)list->data;

	list = list->next;
	ret->created_at = (char*)list->data;

	return ret;
}

t_group* group_create(const char* name, int created_by)
{
	t_group* ret = (t_group*)malloc(sizeof(t_group));

	ret->id = 0;

	ret->name = mx_strdup(name);

	ret->created_by = created_by;

	ret->creator_username = NULL;

	ret->created_at = NULL;

	return ret;
}

t_list* group_list_from_data_list(t_list* list)
{
	int size = mx_list_size(list) / 5;
	if (size < 1) return NULL;
	t_list* ret = NULL;

	for (int i = 0; i < size; i++)
	{
		t_group* g = (t_group*)malloc(sizeof(t_group));

		g->id = mx_atoi((char*)list->data);

		list = list->next;
		g->name = (char*)list->data;

		list = list->next;
		g->created_by = mx_atoi((char*)list->data);

		list = list->next;
		g->creator_username = (char*)list->data;

		list = list->next;
		g->created_at = (char*)list->data;

		list = list->next;

		mx_push_back(&ret, g);
	}

	return ret;
}

void free_group(t_group** group)
{
	if (!*group) return;
	mx_strdel(&(*group)->name);
	mx_strdel(&(*group)->creator_username);
	mx_strdel(&(*group)->created_at);
	free(*group);
}

void free_group_list(t_list* list)
{
	if (!list) return;
	int size = mx_list_size(list);
	for (int i = 0; i < size; i++) {
		t_group* g = (t_group*)mx_list_element_at(list, i);
		free_group(&g);
	}
	mx_del_list(list, size);
}