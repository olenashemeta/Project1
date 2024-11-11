#include "../../../../inc/server.h"

void database_delete(const char* from, const char* where) {
    sqlite3* db;
    char* error = NULL;
    char* command = NULL;
    int rc = sqlite3_open(DB_NAME, &db);

    validate_database_operation(rc, db, NULL);

    if (where == NULL) {
        asprintf(&command, "DELETE FROM %s", from);
    }
    else {
        asprintf(&command, "DELETE FROM %s WHERE %s", from, where);
    }
    rc = sqlite3_exec(db, command, 0, 0, &error);
    validate_database_operation(rc, db, error);

    mx_strdel(&command);
    sqlite3_close(db);
}