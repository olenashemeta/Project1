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

int mx_receiving_pubkey(t_main *main) {
    if (!main || main->socket < 0) {
        fprintf(stderr, "Invalid arguments or socket\n");
        return -1;
    }

    char buffer[2048];
    ssize_t received = recv(main->socket, buffer, sizeof(buffer), 0);

    if (received <= 0) {
        perror("Failed to receive public key");
        return -1;
    }

    BIO *bio = BIO_new_mem_buf(buffer, received);
    if (!bio) {
        perror("Failed to create BIO");
        return -1;
    }

    main->keys.pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    if (main->keys.pkey == NULL) {
        ERR_print_errors_fp(stderr);
        BIO_free(bio);
        return -1;
    }

    BIO_free(bio);
    printf("Public key received and parsed successfully.\n");
    return 0;
}

int mx_transfer_aes_key(t_main *main) {
    if (!main || !main->keys.aes_key || !main->keys.aes_iv || !main->keys.pkey) {
        fprintf(stderr, "Invalid input: missing AES keys or public key\n");
        return -1;
    }

    cJSON *json_transfer_key = form_aes_key_transfer(main->keys.aes_key, main->keys.aes_iv, main->keys.pkey);
    if (!json_transfer_key) {
        fprintf(stderr, "Failed to create key transfer request JSON\n");
        return -1;  
    }

    char *json_str = cJSON_PrintUnformatted(json_transfer_key);
    if (!json_str) {
        fprintf(stderr, "Failed to serialize JSON\n");
        cJSON_Delete(json_transfer_key);
        return -1;
    }

    ssize_t sent_bytes = send(main->socket, json_str, strlen(json_str), 0);
    if (sent_bytes == -1) {
        perror("Failed to send key transfer request to server");
        free(json_str);
        cJSON_Delete(json_transfer_key);
        return -1;
    }

    if ((size_t)sent_bytes != strlen(json_str)) {
        fprintf(stderr, "Incomplete data sent to server\n");
        free(json_str);
        cJSON_Delete(json_transfer_key);
        return -1;
    }

    printf("AES key and IV successfully sent to the server.\n");

    free(json_str);
    cJSON_Delete(json_transfer_key);
    return 0;
}
