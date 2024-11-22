#include "../inc/server.h"

void *handle_client(void *arg) {
    t_client *client = (t_client *)arg;

    syslog(LOG_INFO, "Client connected with ID: %ld", client->thread_id);

    if (mx_send_pubkey(client) > 0 || mx_recieve_aes(client) > 0) {
        syslog(LOG_ERR, "Key exchange failed for client ID: %ld", client->thread_id);
        close(client->socket_fd);
        free(client);
        pthread_exit(NULL);
    }

    while (1) {
        char buffer[4096];
        ssize_t received = recv(client->socket_fd, buffer, sizeof(buffer) - 1, 0);

        if (received <= 0) {
            syslog(LOG_INFO, "Client ID %ld disconnected.", client->thread_id);
            break;
        }

        buffer[received] = '\0';

        cJSON *json = cJSON_Parse(buffer);
        if (!json) {
            syslog(LOG_WARNING, "Invalid JSON received from client ID: %ld", client->thread_id);
            continue;
        }
        mx_process_client_request(json);
        cJSON_Delete(json);
    }

    free_client(client);
    pthread_exit(NULL);
}
