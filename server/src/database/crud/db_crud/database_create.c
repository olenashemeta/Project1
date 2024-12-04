#include "../../../../inc/server.h"

int database_create(const char* insert, const char* into, const char* values) {
    sqlite3* db;
    char* error = NULL;
    char* command = NULL;
    int rc = sqlite3_open(exe_path, &db);

    if(!validate_database_operation(rc, db, NULL)) return -1;

    asprintf(&command, "INSERT INTO %s(%s) VALUES (%s)", into, insert, values);
    rc = sqlite3_exec(db, command, 0, 0, &error);
    if(!validate_database_operation(rc, db, error)) return -1;

    int res = sqlite3_last_insert_rowid(db);
    mx_strdel(&command);
    sqlite3_close(db);

    return res;
}
