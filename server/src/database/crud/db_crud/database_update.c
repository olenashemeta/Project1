#include "../../../../inc/server.h"

bool database_update(const char* update, const char* set, const char* where) {
    sqlite3* db;
    char* error = NULL;
    char* command = NULL;
    int rc = sqlite3_open(exe_path, &db);

    if(!validate_database_operation(rc, db, NULL)) return false;

    if (where == NULL) {
        asprintf(&command, "UPDATE %s SET %s", update, set);
    }
    else {
        asprintf(&command, "UPDATE %s SET %s WHERE %s", update, set, where);
    }
    rc = sqlite3_exec(db, command, 0, 0, &error);
    
    if(!validate_database_operation(rc, db, error)) return false;

    mx_strdel(&command);
    sqlite3_close(db);

    return true;
}
