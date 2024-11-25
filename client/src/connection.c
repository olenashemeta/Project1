#include "../inc/client.h"

int mx_connect_to_server(t_main *main) {
    int sock;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(main->port);

    if (inet_pton(AF_INET, main->address, &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported \n");
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return -1;
    }

    return sock;
}

int mx_receiving_pubkey(int socket_fd, EVP_PKEY **pubkey) {
    char buffer[4096];
    ssize_t received = recv(socket_fd, buffer, sizeof(buffer), 0);

    if (received <= 0) {
        perror("Failed to receive public key\n");;
        return -1;
    }

    BIO *bio = BIO_new_mem_buf(buffer, received);
    if (!bio) {
        perror("Failed to create BIO\n");
        return -1;
    }

    *pubkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    if (*pubkey == NULL) {
        ERR_print_errors_fp(stderr);
        BIO_free(bio);
        return -1;
    }

    BIO_free(bio);
    printf("Public key received and parsed successfully.\n");
    return 0;
}


int mx_transfer_aes_key(const unsigned char *aes_key, const unsigned char *iv, int sock, EVP_PKEY *pubkey) {
    cJSON *json_transfer_key = form_aes_key_transfer(aes_key, iv, pubkey);
    
    if (!json_transfer_key) {
        fprintf(stderr, "Failed to create key transfer request JSON\n");
        return 1;
    }
        
    char *json_str = cJSON_PrintUnformatted(json_transfer_key);
    if (!json_str) {
        fprintf(stderr, "Failed to print JSON\n");
        cJSON_Delete(json_transfer_key);
        return 1;
    }

    if (send(sock, json_str, strlen(json_str), 0) == -1) {
        perror("Failed to send key transfer request to server");
        return 1;
    }

    free(json_str);
    cJSON_Delete(json_transfer_key);
    return 0;
}
