#include "../../../../inc/server.h"

int database_create(const char* insert, const char* into, const char* values) {
    sqlite3* db;
    char* error = NULL;
    char* command = NULL;
    int rc = sqlite3_open(DB_NAME, &db);

    validate_database_operation(rc, db, NULL);

    asprintf(&command, "INSERT INTO %s(%s) VALUES (%s)", into, insert, values);
    rc = sqlite3_exec(db, command, 0, 0, &error);
    validate_database_operation(rc, db, error);

    int res = sqlite3_last_insert_rowid(db);
    mx_strdel(&command);
    sqlite3_close(db);

    return res;
}