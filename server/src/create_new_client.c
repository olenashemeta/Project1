#include "../inc/server.h"

t_client *create_new_client(int socket_fd) {
    t_client *client = malloc(sizeof(t_client));

    client->socket_fd = socket_fd;
    client->thread_id = 0;

    return client;
}

void free_client(t_client *client) {
    if (client) {

        if (client->socket_fd >= 0) {
            close(client->socket_fd);
        }

        free(client);
    }
}
