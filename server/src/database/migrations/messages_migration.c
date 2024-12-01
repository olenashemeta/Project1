#include "../../../inc/server.h"

void messages_migration_up(void) {
    sqlite3* db;
    char* error = NULL;
    char* command = NULL;
    int rc = sqlite3_open(DB_NAME, &db);

    validate_database_operation(rc, db, NULL);

    rc = sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS messages ("
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "sent_by INTEGER NOT NULL,"
        "text VARCHAR(255) NOT NULL,"
        "created_at DATETIME NOT NULL,"
        "group_id INTEGER NOT NULL,"
        "FOREIGN KEY(sent_by) REFERENCES users(id),"
        "FOREIGN KEY(group_id) REFERENCES groups(id))"
        , 0, 0, &error);
    validate_database_operation(rc, db, error);

    sqlite3_close(db);
}

void messages_migration_down(void) {
    sqlite3* db;
    char* error = NULL;
    char* command = NULL;
    int rc = sqlite3_open(DB_NAME, &db);

    validate_database_operation(rc, db, NULL);

    rc = sqlite3_exec(db,
        "DROP TABLE IF EXISTS messages", 0, 0, &error);

    validate_database_operation(rc, db, error);

    sqlite3_close(db);
}
