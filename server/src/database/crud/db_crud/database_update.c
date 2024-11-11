#include "../../../../inc/server.h"

void database_update(const char* update, const char* set, const char* where) {
    sqlite3* db;
    char* error = NULL;
    char* command = NULL;
    int rc = sqlite3_open(DB_NAME, &db);

    validate_database_operation(rc, db, NULL);

    if (where == NULL) {
        asprintf(&command, "UPDATE %s SET %s", update, set);
    }
    else {
        asprintf(&command, "UPDATE %s SET %s WHERE %s", update, set, where);
    }
    rc = sqlite3_exec(db, command, 0, 0, &error);
    validate_database_operation(rc, db, error);

    mx_strdel(&command);
    sqlite3_close(db);
}