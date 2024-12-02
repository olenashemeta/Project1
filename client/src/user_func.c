#include "../inc/client.h"

t_user* mx_create_user(const char* login, const char* name, const char* password) {
    t_user* new_user = (t_user*)malloc(sizeof(t_user));

    if (new_user == NULL) {
        return NULL;  
    }

    new_user->login = mx_strdup(login);
    new_user->name = mx_strdup(name);
    new_user->password = mx_strdup(password);

    return new_user;
}

t_user* mx_create_log_user(const char* login, const char* password) {
    t_user* new_user = (t_user*)malloc(sizeof(t_user));

    if (new_user == NULL) {
        return NULL; 
    }

    new_user->login = mx_strdup(login);
    new_user->password = mx_strdup(password);
    new_user->name = NULL; 

    return new_user;
}

void mx_free_user(t_user* new_user) {
    if (new_user != NULL) {
        if (new_user->login != NULL) {
            free(new_user->login);
        }
        if (new_user->name != NULL) {
            free(new_user->name);
        }
        if (new_user->password != NULL) {
            free(new_user->password);
        }
        free(new_user);
    }
}
