#include "../../../inc/server.h"

void users_migration_up() {
    sqlite3* db;
    char* error = NULL;
    char* command = NULL;
    int rc = sqlite3_open(DB_NAME, &db);

    validate_database_operation(rc, db, NULL);

    rc = sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "username VARCHAR(255) NOT NULL,"
        "login VARCHAR(255) NOT NULL UNIQUE,"
        "password VARCHAR(255) NOT NULL,"
        "created_at DATETIME NOT NULL)", 0, 0, &error);
    
    validate_database_operation(rc, db, error);

    sqlite3_close(db);
}

void users_migration_down() {
    sqlite3* db;
    char* error = NULL;
    char* command = NULL;
    int rc = sqlite3_open(DB_NAME, &db);

    validate_database_operation(rc, db, NULL);

    rc = sqlite3_exec(db,
        "DROP TABLE IF EXISTS users", 0, 0, &error);

    validate_database_operation(rc, db, error);

    sqlite3_close(db);
}