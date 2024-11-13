#include "../../inc/server.h"

t_message* message_from_data_list(t_list* list)
{
	if (mx_list_size(list) < 6)return NULL;

	t_message* ret = (t_message*)malloc(sizeof(t_message));

	ret->id = mx_atoi((char*)list->data);

	list = list->next;
	ret->sent_by = mx_atoi((char*)list->data);

	list = list->next;
	ret->sender_username = (char*)list->data;

	list = list->next;
	ret->text = (char*)list->data;

	list = list->next;
	ret->group_id = mx_atoi((char*)list->data);

	list = list->next;
	ret->created_at = (char*)list->data;

	return ret;
}

t_message* message_create(int sent_by, const char* text, int group_id)
{
	t_message* ret = (t_message*)malloc(sizeof(t_message));

	ret->id = 0;

	ret->sent_by = sent_by;

	ret->sender_username = NULL;

	ret->text = mx_strdup(text);

	ret->group_id = group_id;

	ret->created_at = NULL;

	return ret;
}

t_list* message_list_from_data_list(t_list* list)
{
	int size = mx_list_size(list) / 6;
	if (size < 1) return NULL;
	t_list* ret = NULL;

	for (int i = 0; i < size; i++)
	{
		t_message* m = (t_message*)malloc(sizeof(t_message));

		m->id = mx_atoi((char*)list->data);

		list = list->next;
		m->sent_by = mx_atoi((char*)list->data);

		list = list->next;
		m->sender_username = (char*)list->data;

		list = list->next;
		m->text = (char*)list->data;

		list = list->next;
		m->group_id = mx_atoi((char*)list->data);

		list = list->next;
		m->created_at = (char*)list->data;

		list = list->next;

		mx_push_back(&ret, m);
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
