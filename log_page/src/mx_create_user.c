#include "../inc/log_page.h"

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