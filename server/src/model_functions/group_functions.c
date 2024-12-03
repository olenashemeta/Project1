#include "../../inc/server.h"

t_group* group_from_data_list(t_list* list)
{
	if (mx_list_size(list) < 7)return NULL;

	t_group* ret = (t_group*)malloc(sizeof(t_group));

	ret->id = mx_atoi((char*)list->data);

	list = list->next;
	ret->name = (char*)list->data;

	list = list->next;
	ret->is_private = mx_atoi((char*)list->data);

	list = list->next;
	ret->created_by = mx_atoi((char*)list->data);

	list = list->next;
	ret->creator_username = (char*)list->data;

	list = list->next;
	ret->created_at = (char*)list->data;

	list = list->next;
	ret->last_message_date = (char*)list->data;

	return ret;
}

t_group* group_create(const char* name, int created_by, int is_private)
{
	t_group* ret = (t_group*)malloc(sizeof(t_group));

	ret->id = 0;

	ret->name = mx_strdup(name);

	ret->is_private = is_private;

	ret->created_by = created_by;

	ret->creator_username = NULL;

	ret->created_at = NULL;

	ret->last_message_date = NULL;

	return ret;
}

t_list* group_list_from_data_list(t_list* list)
{
	int size = mx_list_size(list) / 7;
	if (size < 1) return NULL;
	t_list* ret = NULL;

	for (int i = 0; i < size; i++)
	{
		t_group* g = (t_group*)malloc(sizeof(t_group));

		g->id = mx_atoi((char*)list->data);

		list = list->next;
		g->name = (char*)list->data;

		list = list->next;
		g->is_private = mx_atoi((char*)list->data);
		
		list = list->next;
		g->created_by = mx_atoi((char*)list->data);

		list = list->next;
		g->creator_username = (char*)list->data;

		list = list->next;
		g->created_at = (char*)list->data;

		list = list->next;
		g->last_message_date = (char*)list->data;

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
	mx_strdel(&(*group)->last_message_date);
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

cJSON *group_to_json(t_group* group) {
	if(!group) return NULL;
	cJSON* json = cJSON_CreateObject();
	
	if(!json_payload) return NULL;

	cJSON_AddNumberToObject(json, "id", (const double)group->id);
	cJSON_AddStringToObject(json, "name", group->name);
	cJSON_AddBoolToObject(json, "is_private", group->is_private == 1);
	cJSON_AddNumberToObject(json, "created_by", (const double)group->created_by);
	cJSON_AddStringToObject(json, "creator_username", group->creator_username);
	cJSON_AddStringToObject(json, "created_at", user->created_at);
	cJSON_AddStringToObject(json, "last_message_date", group->last_message_date);

	return json;
}

cJSON *groups_list_to_json_array(t_list* list) {
	cJSON *array = cJSON_CreateArray();
	if(!array) return NULL;
	while(list) {
		cJSON *item = group_to_json((t_group *)list->data);
		if(!item)break;
		cJSON_AddItemToArray(array, item);
	}
	return array;
}
