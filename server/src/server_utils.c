#include "../inc/server.h"

t_server *create_server(void) {
    t_server *server = malloc(sizeof(t_server));
    if (!server) {
        perror("Failed to allocate memory for server");
        exit(EXIT_FAILURE);
    }

    server->sd = -1;
    server->ai = NULL;
    server->is_running = false;

    return server;
}

void free_server(t_server *server) {
    if (!server) return;

    if (server->sd != -1) {
        close(server->sd);
        server->sd = -1;
    }

    if (server->ai) {
        freeaddrinfo(server->ai);
        server->ai = NULL;
    }

    free(server);
}
    