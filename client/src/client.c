#include "../inc/client.h"

static void *connection(void *arg) {
    t_main *main = (t_main*)arg;
    main->socket = mx_connect_to_server(main);
        if (main->socket == -1) {
        perror("Failed to connect to server");
        return NULL;
    }
    printf("Connected to server.\n");
    if (mx_receiving_pubkey(main->socket, &main->pubkey) == -1) {
        perror("Failed to receive public key from server");
        close(main->socket);
        return NULL;
    }

    printf("Received public key:\n");
    if (!PEM_write_PUBKEY(stdout, main->pubkey)) {
        fprintf(stderr, "Failed to write public key\n");
        ERR_print_errors_fp(stderr);
    }

    while (1) {
        char buffer[4096];
        ssize_t bytes_received = recv(main->socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) break;

        buffer[bytes_received] = '\0';
        cJSON *json_response = cJSON_Parse(buffer);

        pthread_mutex_lock(&main->lock);
        cJSON_Delete(main->server_response);
        main->server_response = json_response;
        main->has_new_data = true;
        pthread_cond_signal(&main->cond);
        pthread_mutex_unlock(&main->lock);
    }

    close(main->socket);
    return NULL;
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

