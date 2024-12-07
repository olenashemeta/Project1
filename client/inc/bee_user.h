#ifndef BEE_USER_H
#define BEE_USER_H

#include "../inc/client.h"

typedef struct s_bee_user {
    //int id;
    int logo_id;
    char *username;
    char *login;
    char *created_at;
}              t_bee_user;

t_bee_user *create_bee_user_from_json(cJSON *data);
void free_bee_user(t_bee_user *user);

#endif
