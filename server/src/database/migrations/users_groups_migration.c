#include "../../../inc/server.h"

void users_groups_migration_up(void) {
    sqlite3* db;
    char* error = NULL;
    int rc = sqlite3_open(exe_path, &db);

    if(!validate_database_operation(rc, db, NULL)) return;

    rc = sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS users_groups ("
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "created_at DATETIME NOT NULL,"
        "user_id INTEGER NOT NULL,"
        "group_id INTEGER NOT NULL,"
        "FOREIGN KEY(user_id) REFERENCES users(id),"
        "FOREIGN KEY(group_id) REFERENCES groups(id),"
        "CONSTRAINT UC_users_groups UNIQUE (user_id, group_id))"
        , 0, 0, &error);

    validate_database_operation(rc, db, error);

    sqlite3_close(db);
}

void users_groups_migration_down(void) {
    sqlite3* db;
    char* error = NULL;
    int rc = sqlite3_open(exe_path, &db);

    if(!validate_database_operation(rc, db, NULL)) return;

    rc = sqlite3_exec(db,
        "DROP TABLE IF EXISTS users_groups", 0, 0, &error);

    validate_database_operation(rc, db, error);

    sqlite3_close(db);
}
