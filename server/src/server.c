#include "../inc/server.h"

void *handle_client(void *arg) {
    t_client *client = (t_client *)arg;

    syslog(LOG_INFO, "Client connected with ID: %d", client->client_id);

    if (mx_send_pubkey(client) > 0 || mx_recieve_aes(client) > 0) {
        syslog(LOG_ERR, "Key exchange failed for client ID: %d", client->client_id);
        close(client->socket_fd);
        free(client);
        pthread_exit(NULL);
    }

    while (1) {
        char buffer[4096];
        ssize_t received = recv(client->socket_fd, buffer, sizeof(buffer) - 1, 0);

        if (received <= 0) {
            syslog(LOG_INFO, "Client ID %d disconnected.", client->client_id);
            break;
        }

        buffer[received] = '\0';

        cJSON *json = cJSON_Parse(buffer);
        if (!json) {
            syslog(LOG_WARNING, "Invalid JSON received from client ID: %d", client->client_id);
            continue;
        }
        mx_process_client_request(json);
        cJSON_Delete(json);
    }

    close(client->socket_fd);
    free(client);
    pthread_exit(NULL);
}

void start_server(int port) {
    int server_fd = -1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Server listening on port %d", port);

    int client_id = 1;
    while (1) {
        t_client *client = malloc(sizeof(t_client));
        if (!client) {
            syslog(LOG_ERR, "Memory allocation failed for new client");
            continue;
        }

        client->socket_fd = accept(server_fd, (struct sockaddr *)&client->address, (socklen_t *)&addrlen);
        if (client->socket_fd == -1) {
            perror("Accept failed");
            free(client);
            continue;
        }

        client->client_id = client_id++;

        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, client) != 0) {
            perror("Thread creation failed");
            close(client->socket_fd);
            free(client);
            continue;
        }

        pthread_detach(client_thread);
    }

    close(server_fd);
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "migration_down") == 0) {
        migration_down();
        return 1;
    } else {
        migration_up();
    }

    mx_daemon_start();
    set_signal();

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd))) {
        syslog(LOG_INFO, "Current working directory: %s", cwd);
    } else {
        perror("getcwd failed");
    }

    start_server(8080);

    return 0;
}