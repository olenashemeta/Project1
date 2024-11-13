#include "../../../../inc/server.h"

int db_message_create(t_message* message)
{
	char* values = NULL;
	asprintf(&values, "%d, '%s', %d, datetime()", message->sent_by, message->text, message->group_id);

	int ret = database_create("sent_by, text, group_id, created_at", "messages", values);

	mx_strdel(&values);

	return ret;
}

void db_message_delete(t_message* message)
{
	char* where = NULL;
	asprintf(&where, "id = %d", message->id);

	database_delete("messages", where);

	mx_strdel(&where);
}

void db_message_delete_by_id(int id)
{
	char* where = NULL;
	asprintf(&where, "id = %d", id);

	database_delete("messages", where);

	mx_strdel(&where);
}

void db_message_delete_by_named_field(const char* field, const char* value)
{
	char* where = NULL;
	asprintf(&where, "%s = '%s'", field, value);

	database_delete("messages", where);

	mx_strdel(&where);
}

void db_message_update(t_message* message)
{
	char* where = NULL;
	char* set = NULL;
	asprintf(&where, "id = %d", message->id);
	asprintf(&set, "sent_by = %d, text = '%s', group_id = %d", message->sent_by, message->text, message->group_id);
	database_update("messages", set, where);

	mx_strdel(&where);
	mx_strdel(&set);
}

t_message* db_message_read_by_id(int id) 
{
	char* where = NULL;
	asprintf(&where, "messages.id = %d", id);

	t_list* list = database_read("messages.id, sent_by, users.username, text, group_id, messages.created_at", "messages INNER JOIN users ON sent_by = users.id", where);
	t_message* ret = message_from_data_list(list);

	mx_del_list(list, mx_list_size(list));

	mx_strdel(&where);

	return ret;
}

t_list* db_message_read_all() 
{

	t_list* list = database_read("messages.id, sent_by, users.username, text, group_id, messages.created_at", "messages INNER JOIN users ON sent_by = users.id", NULL);
	t_list* ret = message_list_from_data_list(list);

	mx_del_list(list, mx_list_size(list));

	return ret;
}

t_list* db_message_read_by_sender_id(int id)
{
	char* where = NULL;
	asprintf(&where, "messages.sent_by = %d", id);

	t_list* list = database_read("messages.id, sent_by, users.username, text, group_id, messages.created_at", "messages INNER JOIN users ON sent_by = users.id", where);
	t_list* ret = message_list_from_data_list(list);

	mx_del_list(list, mx_list_size(list));

	mx_strdel(&where);

	return ret;
}

t_list* db_message_read_by_group_id(int id)
{
	char* where = NULL;
	asprintf(&where, "messages.group_id = %d", id);

	t_list* list = database_read("messages.id, sent_by, users.username, text, group_id, messages.created_at", "messages INNER JOIN users ON sent_by = users.id", where);
	t_list* ret = message_list_from_data_list(list);

	mx_del_list(list, mx_list_size(list));

	mx_strdel(&where);

	return ret;
}
