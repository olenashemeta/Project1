#ifndef SERVER_H
#define SERVER_H

#include "../lib/libmx/inc/libmx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sqlite3.h>

//models
typedef struct s_user {
	int id;
	char* username;
	char* login;
	char* password;
	char* created_at;
} t_user;

typedef struct s_message {
	int id;
	int sent_by;
	char* sender_username;
	char* text;
	int group_id;
	char* created_at;
} t_message;

typedef struct s_group {
	int id;
	char* name;
	int created_by;
	char* creator_username;
	char* created_at;
} t_group;

//Migrations
void users_migration_up();
void users_migration_down();
void groups_migration_up();
void groups_migration_down();
void messages_migration_up();
void messages_migration_down();
void users_groups_migration_up();
void users_groups_migration_down();
void migration_up();
void migration_down();

//CRUD Operations
int database_create(const char* insert, const char* into, const char* values);
t_list* database_read(const char* select, const char* from, const char* where);
void database_update(const char* update, const char* set, const char* where);
void database_delete(const char* from, const char* where);

//Users CRUD
int db_user_create(t_user* user);
void db_user_delete(t_user* user);
void db_user_delete_by_id(int id);
void db_user_delete_by_named_field(const char* field, const char* value);
void db_user_update(t_user* user);
t_user* db_user_read_by_id(int id);
t_list* db_user_read_all();

//Error handling
void validate_database_operation(int rc, sqlite3* db, char* error);

//Model functions
t_user* user_create(const char* username, const char* login, const char* password);
t_user* user_from_data_list(t_list* list);
t_list* user_list_from_data_list(t_list* list);
void free_user(t_user** user);
void free_user_list(t_list* list);

#endif 