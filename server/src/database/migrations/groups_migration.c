#include "../../../inc/server.h"

void groups_migration_up(void) {
    sqlite3* db;
    char* error = NULL;
    int rc = sqlite3_open(exe_path, &db);

    validate_database_operation(rc, db, NULL);

    rc = sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS groups ("
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "name VARCHAR(255) NOT NULL,"
        "is_private INTEGER DEFAULT 1, "
        "created_at DATETIME NOT NULL,"
        "created_by INTEGER NOT NULL,"
        "FOREIGN KEY(created_by) REFERENCES users(id))"
        , 0, 0, &error);

    validate_database_operation(rc, db, error);

    sqlite3_close(db);
}

void groups_migration_down(void) {
    sqlite3* db;
    char* error = NULL;
    int rc = sqlite3_open(exe_path, &db);

    validate_database_operation(rc, db, NULL);

    rc = sqlite3_exec(db,
        "DROP TABLE IF EXISTS groups", 0, 0, &error);

    validate_database_operation(rc, db, error);

    sqlite3_close(db);
}
