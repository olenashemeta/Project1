#include "../inc/client.h"

t_group* create_group(const char* name, int is_private, const char* creator_username) {
    t_group* new_group = (t_group*)malloc(sizeof(t_group));

    new_group->name = (char*)malloc(strlen(name) + 1);
    strcpy(new_group->name, name);

    new_group->is_private = is_private;

    new_group->creator_username = (char*)malloc(strlen(creator_username) + 1);
    strcpy(new_group->creator_username, creator_username);

    new_group->sender = NULL;

    return new_group;
}

void free_group(t_group* group) {
    if (group) {
        if (group->name) {
            free(group->name);
        }
        if (group->creator_username) {
            free(group->creator_username);
        }
        free(group);
    }
}
