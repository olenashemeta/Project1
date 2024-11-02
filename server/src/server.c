#include "../inc/server.h"

void *handle_client(void *arg) {
    t_client *client = (t_client *)arg;
    syslog(LOG_INFO, "Client connected with ID: %d", client->client_id);

    close(client->socket_fd);
    free(client);
    pthread_exit(NULL);
}

int main(void) {
    int server_fd, server_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    mx_daemon_start();
    set_signal();
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;            // IPv4
    address.sin_addr.s_addr = INADDR_ANY;     // Принимать подключения от любого IP
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8080\n");
    int id = 1;
    while(1) {
        t_client *client = malloc(sizeof(t_client));

        client->socket_fd = accept(server_fd, (struct sockaddr *)&client->address, (socklen_t*)&addrlen);
        if (client->socket_fd < 0) {
            perror("accept");
            free(client);
            continue;
        }

        client->client_id = id++;

        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, (void *)client) != 0) {
            perror("pthread_create failed");
            close(client->socket_fd);
            free(client);
            continue;
        }

        pthread_detach(client_thread);
    }

    close(server_socket);
    close(server_fd);
    return 0;
}
