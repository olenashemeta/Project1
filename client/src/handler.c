#include "../inc/client.h"
void *connection(void *arg) {
    t_main *main = (t_main *)arg;

    while (!main->is_closing) {
        main->socket = mx_connect_to_server(main);
        if (main->socket == -1) {
            printf("Failed to connect to server. Retrying in %d seconds...\n", main->rec_delay);
            sleep(main->rec_delay);
            continue;
        }

        printf("Connected to server.\n");
        main->is_connected = true;

        if (handshake(main) != 0) {
            fprintf(stderr, "Handshake failed. Retrying connection...\n");
            close(main->socket);
            main->is_connected = false;
            continue;
        }

        printf("Handshake succeeded. AES session established.\n");

        while (!main->is_closing) {
            char buffer[4096];
            ssize_t bytes_received = recv(main->socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0) {
                fprintf(stderr, "Connection lost. Reconnecting...\n");
                close(main->socket);
                main->is_connected = false;
                break;
            }

            buffer[bytes_received] = '\0';

            cJSON *json_response = cJSON_Parse(buffer);
            if (!json_response) {
                fprintf(stderr, "Failed to parse JSON response from server.\n");
                continue;
            }

            pthread_mutex_lock(&main->lock);
            cJSON_Delete(main->server_response);
            main->server_response = json_response;
            main->has_new_data = true;
            pthread_cond_signal(&main->cond);
            pthread_mutex_unlock(&main->lock);
        }

        close(main->socket);
        main->socket = -1;
    }

    printf("Closing connection thread.\n");
    pthread_exit(NULL);
}
