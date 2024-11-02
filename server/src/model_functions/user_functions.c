#include "../../inc/server.h"

t_user *user_from_data_list(t_list *list)
{
	if (mx_list_size(list) < 5)return NULL;

	t_user* ret = (t_user*)malloc(sizeof(t_user));

	ret->id = mx_atoi((char *)list->data);
	
	list = list->next;
	ret->username = (char *)list->data;

	list = list->next;
	ret->login = (char*)list->data;

	list = list->next;
	ret->password = (char*)list->data;

	list = list->next;
	ret->created_at = (char*)list->data;

	return ret;
}

t_user* user_create(const char* username, const char* login, const char* password) 
{
	t_user* ret = (t_user*)malloc(sizeof(t_user));

	ret->id = 0;

	ret->username = mx_strdup(username);

	ret->login = mx_strdup(login);

	ret->password = mx_strdup(password);

	ret->created_at = NULL;

	return ret;
}

t_list *user_list_from_data_list(t_list* list) 
{
	int size = mx_list_size(list) / 5;
	if (size < 1) return NULL;
	t_list* ret = NULL;

	for (int i = 0; i < size; i++) 
	{
		t_user *u = (t_user*)malloc(sizeof(t_user));

		u->id = mx_atoi((char*)list->data);

		list = list->next;
		u->username = (char*)list->data;

		list = list->next;
		u->login = (char*)list->data;

		list = list->next;
		u->password = (char*)list->data;

		list = list->next;
		u->created_at = (char*)list->data;

		list = list->next;

		mx_push_back(&ret, u);
	}

	return ret;
}

void free_user(t_user** user)
{
	mx_strdel(&(*user)->login);
	mx_strdel(&(*user)->password);
	mx_strdel(&(*user)->username);
	mx_strdel(&(*user)->created_at);
	free(*user);
}

void free_user_list(t_list* list)
{
	int size = mx_list_size(list);
	for (int i = 0; i < size; i++) {
		t_user* u = (t_user*)mx_list_element_at(list, i);
		free_user(&u);
	}
	mx_del_list(list, size);
}