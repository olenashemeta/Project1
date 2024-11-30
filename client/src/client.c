#include "../inc/client.h"

static void *connection(void *arg) {
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

        t_packet *received_data = NULL;
        while (!main->is_closing && (received_data = receive_request(main->socket)) != NULL) {
            if (decrypt_received_data(received_data, main->keys.aes_key, main->keys.aes_iv) == -1) {
                fprintf(stderr, "Failed to decrypt data\n");
                free_receive(received_data);
                break;
            }

            process_response(received_data);

            free_receive(received_data);

            // pthread_mutex_lock(&main->lock);
            // cJSON_Delete(main->server_response);
            // main->server_response = json_response;
            // main->has_new_data = true;
            // pthread_cond_signal(&main->cond);
            // pthread_mutex_unlock(&main->lock);
        }

        close(main->socket);
        main->socket = -1;
    }

    printf("Closing connection thread.\n");
    pthread_exit(NULL);
}

static void activate(GtkApplication *app, gpointer user_data) {
    t_main *main = (t_main*)user_data;
    if (main) {
        pthread_t thread_id;
        if ((pthread_create(&thread_id, NULL, connection, main)) != 0){
            return;
        }
        login_window(app, user_data);
    }
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    t_main *main = mx_create_main_data("127.0.0.1", 8080);

    app = gtk_application_new("app.chat", 0);
    g_signal_connect(app, "activate", G_CALLBACK (activate), main);
    status = g_application_run (G_APPLICATION (app), argc, argv);

    g_object_unref(app);
    mx_free_main_data(main);
    return status;
}

