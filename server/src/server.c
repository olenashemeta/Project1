#include "../inc/server.h"

void mx_send_pubkey(void *arg) {
    t_client *client = (t_client *)arg;

    FILE *pubkey_file = fopen("public_key.pem", "r");
    if (!pubkey_file) {
        syslog(LOG_INFO, "Unable to open public key file");
        close(client->socket_fd);
        free(client);
        return;
    }

    EVP_PKEY *pubkey = PEM_read_PUBKEY(pubkey_file, NULL, NULL, NULL);
    fclose(pubkey_file);

    if (!pubkey) {
        ERR_print_errors_fp(stderr);
        close(client->socket_fd);
        free(client);
        return;
    }

    BIO *bio = BIO_new(BIO_s_mem());
    if (!bio) {
        syslog(LOG_INFO, "Unable to create BIO");
        EVP_PKEY_free(pubkey);
        close(client->socket_fd);
        free(client);
        return;
    }

    if (!PEM_write_bio_PUBKEY(bio, pubkey)) {
        ERR_print_errors_fp(stderr);
        BIO_free(bio);
        EVP_PKEY_free(pubkey);
        close(client->socket_fd);
        free(client);
        return;
    }

    char *pem_data;
    long pem_length = BIO_get_mem_data(bio, &pem_data);

    ssize_t sent = send(client->socket_fd, pem_data, pem_length, 0);
    if (sent == -1) {
        syslog(LOG_INFO, "Failed to send public key");
    } else if (sent != pem_length) {
        syslog(LOG_INFO, "Sent public key to client ID: %d", client->client_id);
        fprintf(stderr, "Warning: Sent %ld of %ld bytes\n", sent, pem_length);
    }

    BIO_free(bio);
    EVP_PKEY_free(pubkey);
}

void *handle_client(void *arg) {
    t_client *client = (t_client *)arg;
    syslog(LOG_INFO, "Client connected with ID: %d", client->client_id);
    mx_send_pubkey(arg);

    while(1) {
        char buffer[4096];
        ssize_t received;

        received = recv(client->socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (received > 0) {
            buffer[received] = '\0';

            cJSON *json = cJSON_Parse(buffer);
            if (json == NULL) {
                perror("Invalid JSON format");
                return NULL;
            }
            mx_process_client_request(json);
        }   
    }

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


    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        syslog(LOG_INFO, "Current working directory: %s", cwd);
    } else {
    perror("getcwd() error");
    }

    
    ////////
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
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
    //////////
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
