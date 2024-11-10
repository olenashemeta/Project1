#include "../inc/client.h"

t_user *mx_create_client(void) {
    t_user *client = malloc(sizeof(t_user));
    client->login = strdup("AAAAAAAAA");
    client->password = strdup("PAROL123456789");

    return client;
}

void mx_print_client(const t_user *client) {
    if (client == NULL) {
        printf("Client data is NULL\n");
        return;
    }
    printf("Login: %s\n", client->login);
    printf("Password: %s\n", client->password);
}

void mx_free_client(t_user *client) {
    if (client == NULL) {
        return;
    }
    free(client->login);
    free(client->password);
    free(client);
}
