#include "../../../../inc/server.h"

int db_user_create(t_user* user) 
{
	char* values = NULL; 
	asprintf(&values, "'%s', '%s', '%s', %d, datetime()", user->username, user->login, user->password, user->logo_id);
	
	int ret = database_create("username, login, password, logo_id, created_at", "users", values);

	mx_strdel(&values);

	return ret;
}

void db_user_delete(t_user* user) 
{
	char* where;
	if (user->id > 0)asprintf(&where, "id = %d", user->id);
	else asprintf(&where, "login = '%s'", user->login);

	database_delete("users", where);

	mx_strdel(&where);
}

void db_user_delete_by_id(int id) 
{
	char* where = NULL;
	asprintf(&where, "id = %d", id);

	database_delete("users", where);

	mx_strdel(&where);
}

void db_user_delete_by_named_field(const char *field, const char *value)
{
	char* where = NULL;
	asprintf(&where, "%s = '%s'", field, value);

	database_delete("users", where);

	mx_strdel(&where);
}

void db_user_update(t_user* user) 
{
	char* where = NULL;
	char* set = NULL;
	asprintf(&where, "id = %d", user->id);
	asprintf(&set, "username = '%s', login = '%s', password = '%s', logo_id = %d", user->username, user->login, user->password, user->logo_id);
	database_update("users", set, where);
	
	mx_strdel(&where);
	mx_strdel(&set);
}

t_user* db_user_read_by_id(int id) {
	char* where = NULL;
	asprintf(&where, "id = %d", id);

	t_list* list = database_read("id, username, login, password, logo_id, created_at", "users", where);
	t_user* ret = user_from_data_list(list);
	
	mx_del_list(list, mx_list_size(list));

	mx_strdel(&where);

	return ret;
}

t_user* db_user_read_by_login(const char *login) {
	char* where = NULL;
	asprintf(&where, "login = '%s'", login);

	t_list* list = database_read("id, username, login, password, logo_id, created_at", "users", where);
	t_user* ret = user_from_data_list(list);
	
	mx_del_list(list, mx_list_size(list));

	mx_strdel(&where);

	return ret;
}

t_list* db_user_read_all(void) {

	t_list* list = database_read("id, username, login, password, logo_id, created_at", "users", NULL);
	t_list* ret = user_list_from_data_list(list);

	mx_del_list(list, mx_list_size(list));

	return ret;
}
