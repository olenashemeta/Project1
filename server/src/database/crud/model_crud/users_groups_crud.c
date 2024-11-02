#include "../../../../inc/server.h"

t_list* db_user_read_by_group_id(int id) {
	char* where = NULL;
	char* from = NULL;
	asprintf(&where, "users_groups.group_id = %d", id);

	t_list* list = database_read("users.id, users.username, users.login, users.password, users.created_at", "users INNER JOIN users_groups ON users.id = users_groups.user_id", where);
	t_list* ret = user_list_from_data_list(list);

	mx_del_list(list, mx_list_size(list));

	mx_strdel(&where);

	return ret;
}

t_list* db_group_read_by_user_id(int id) {
	char* where = NULL;
	char* from = NULL;
	asprintf(&where, "users_groups.user_id = %d", id);

	t_list* list = database_read("groups.id, groups.name, groups.created_by, users.username, groups.created_at", "groups INNER JOIN users_groups ON groups.id = users_groups.group_id INNER JOIN users on users.id = users_groups.user_id", where);
	t_list* ret = group_list_from_data_list(list);

	mx_del_list(list, mx_list_size(list));

	mx_strdel(&where);

	return ret;
}

int db_user_add_to_group(int user_id, int group_id) {
	char* values = NULL;
	asprintf(&values, "datetime(), %d, %d", user_id, group_id);

	int ret = database_create("created_at, user_id, group_id", "users_groups", values);

	mx_strdel(&values);

	return ret;
}

void db_user_remove_from_froup(int user_id, int group_id) {
	char* where = NULL;
	asprintf(&where, "user_id = %d AND group_id = %d", user_id, group_id);

	database_delete("users_groups", where);

	mx_strdel(&where);
}