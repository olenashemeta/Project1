#include "../../inc/server.h"

bool validate_database_operation(int rc, sqlite3* db, char* error) {
    if (rc != SQLITE_OK) {
        if (error != NULL) {
            syslog(LOG_ERR, "Database error: %s\n", error);

            sqlite3_free(error);
        }
        else {
            syslog(LOG_ERR, "Database Error: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_close(db);

        return false;
    }
    return true;
}
