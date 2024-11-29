#include "../inc/log_page.h"

void mx_free_user(t_user* new_user){
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