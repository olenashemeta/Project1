#include "../../../../inc/server.h"

bool database_delete(const char* from, const char* where) {
    sqlite3* db;
    char* error = NULL;
    char* command = NULL;
    int rc = sqlite3_open(exe_path, &db);

    if(!validate_database_operation(rc, db, NULL)) return false;

    if (where == NULL) {
        asprintf(&command, "DELETE FROM %s", from);
    }
    else {
        asprintf(&command, "DELETE FROM %s WHERE %s", from, where);
    }
    rc = sqlite3_exec(db, command, 0, 0, &error);
    
    if(!validate_database_operation(rc, db, error)) return false;

    mx_strdel(&command);
    sqlite3_close(db);

    return true;
}
