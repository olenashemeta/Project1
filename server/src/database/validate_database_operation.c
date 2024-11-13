#include "../../inc/server.h"

void validate_database_operation(int rc, sqlite3* db, char* error) {
    if (rc != SQLITE_OK) {
        if (error != NULL) {
            fprintf(stderr, "Database Error: %s\n", error);

            sqlite3_free(error);
        }
        else {
            fprintf(stderr, "Database Error: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_close(db);
        exit(1);
    }
}
