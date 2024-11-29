#include "../inc/log_page.h"

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
