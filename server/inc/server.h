#ifndef SERVER_H
#define SERVER_H

#define DB_NAME "test.db"
#define AES_KEY_SIZE 32
#define AES_IV_SIZE 16 

#include "../../libs/libmx/inc/libmx.h"
#include "../../libs/JsonLib/cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <sqlite3.h>

typedef struct s_client {
    int client_id;
    int socket_fd;
    struct sockaddr_in address;
}              t_client;

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

//Messages CRUD
int db_message_create(t_message* message);
void db_message_delete(t_message* message);
void db_message_delete_by_id(int id);
void db_message_delete_by_named_field(const char* field, const char* value);
void db_message_update(t_message* message);
t_message* db_message_read_by_id(int id);
t_list* db_message_read_all();
t_list* db_message_read_by_sender_id(int id);
t_list* db_message_read_by_group_id(int id);

//Groups CRUD
int db_group_create(t_group* group);
void db_group_delete(t_group* group);
void db_group_delete_by_id(int id);
void db_group_delete_by_named_field(const char* field, const char* value);
void db_group_update(t_group* group);
t_group* db_group_read_by_id(int id);
t_list* db_group_read_all();

//Users-Groups CRUD
t_list* db_user_read_by_group_id(int id);
t_list* db_group_read_by_user_id(int id);
int db_user_add_to_group(int user_id, int group_id);
void db_user_remove_from_froup(int user_id, int group_id);

//Error handling
void validate_database_operation(int rc, sqlite3* db, char* error);

//Model functions
t_user* user_create(const char* username, const char* login, const char* password);
t_user* user_from_data_list(t_list* list);
t_list* user_list_from_data_list(t_list* list);
void free_user(t_user** user);
void free_user_list(t_list* list);

t_message* message_create(int sent_by, const char* text, int group_id);
t_message* message_from_data_list(t_list* list);
t_list* message_list_from_data_list(t_list* list);
void free_message(t_message** message);
void free_message_list(t_list* list);

t_group* group_from_data_list(t_list* list);
t_group* group_create(const char* name, int created_by);
t_list* group_list_from_data_list(t_list* list);
void free_group(t_group** group);
void free_group_list(t_list* list);

//Server functions
void mx_daemon_start(void);
void mx_daemon_end(int signal);
void set_signal(void);
void mx_process_client_request(cJSON* json);
void handle_login_request(cJSON* json);

int aes_decrypt(const unsigned char* encrypted_data, int encrypted_len, const unsigned char* aes_key,
	const unsigned char* iv, unsigned char* decrypted_data);
int decrypt_aes_key_with_private_key(const unsigned char* encrypted_aes_key, int encrypted_len, unsigned char* aes_key);
int mx_hex_to_bytes(const char* hex_str, unsigned char* out_bytes, size_t max_bytes);

#endif 