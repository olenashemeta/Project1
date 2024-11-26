#include "../inc/client.h"

int generate_aes_key_iv(t_main *main) {
    if (!main) {
        fprintf(stderr, "Invalid main structure\n");
        return -1;
    }

    if (!RAND_bytes(main->keys.aes_key, AES_KEY_SIZE)) {
        fprintf(stderr, "Failed to generate AES key\n");
        return -1;
    }

    if (!RAND_bytes(main->keys.aes_iv, AES_IV_SIZE)) {
        fprintf(stderr, "Failed to generate AES IV\n");
        return -1;
    }

    printf("AES key and IV generated successfully.\n");
    return 0;
}

//test func
static void print_aes_key_hash(const unsigned char *aes_key, size_t length) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(aes_key, length, hash);

    char hex_hash[SHA256_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hex_hash + i * 2, "%02x", hash[i]);
    }
    hex_hash[SHA256_DIGEST_LENGTH * 2] = '\0';

    printf("Computed SHA-256 hash of AES key: %s\n", hex_hash);
}


int handshake(t_main *main) {
    if (!main || main->socket < 0) {
        fprintf(stderr, "Invalid main structure or socket\n");
        return -1;
    }

    if (mx_receiving_pubkey(main) != 0) {
        fprintf(stderr, "Failed to receive server public key\n");
        return -1;
    }

    if (generate_aes_key_iv(main) != 0) {
        fprintf(stderr, "Failed to generate AES keys\n");
        return -1;
    }
    // use func for testing
    print_aes_key_hash(main->keys.aes_key, AES_KEY_SIZE);
   //test_base64_encoding(main);
    if (mx_transfer_aes_key(main) != 0) {
        fprintf(stderr, "Failed to transfer AES keys to the server\n");
        return -1;
    }

    /*
    //Получение подтверждения от сервера 
    char buffer[4096];
    ssize_t received = recv(main->socket, buffer, sizeof(buffer) - 1, 0);
    if (received <= 0) {
        perror("Failed to receive confirmation from server");
        return -1;
    }

    buffer[received] = '\0';
    printf("Server response: %s\n", buffer);

    // обработка ответа сервера
    cJSON *response = cJSON_Parse(buffer);
    if (!response) {
        fprintf(stderr, "Failed to parse server response\n");
        return -1;
    }

    cJSON *status = cJSON_GetObjectItem(response, "status");
    if (!status || !cJSON_IsString(status) || strcmp(status->valuestring, "OK") != 0) {
        fprintf(stderr, "Handshake failed: server returned error\n");
        cJSON_Delete(response);
        return -1;
    }
    printf("Handshake completed successfully.\n");

    cJSON_Delete(response);
    */
    return 0;
}

int aes_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
                unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return -1;

    int len, ciphertext_len;

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int aes_decrypt(const unsigned char *ciphertext, int ciphertext_len, const unsigned char *key, const unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    if (!(ctx = EVP_CIPHER_CTX_new())) {
        fprintf(stderr, "Error initializing decryption context\n");
        return -1;
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        fprintf(stderr, "Error setting decryption parameters\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1) {
        fprintf(stderr, "Error during decryption\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1) {
        fprintf(stderr, "Error finalizing decryption\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

int encrypt_aes_key(EVP_PKEY *pubkey, const unsigned char *aes_key, unsigned char *encrypted_key) {
    if (!pubkey || !aes_key || !encrypted_key) {
        fprintf(stderr, "Invalid argument(s) to encrypt_aes_key\n");
        return -1;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pubkey, NULL);
    if (!ctx) {
        fprintf(stderr, "Failed to create EVP_PKEY_CTX\n");
        return -1;
    }

    size_t encrypted_key_len;
    if (EVP_PKEY_encrypt_init(ctx) <= 0) {
        fprintf(stderr, "EVP_PKEY_encrypt_init failed\n");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_encrypt(ctx, NULL, &encrypted_key_len, aes_key, AES_KEY_SIZE) <= 0) {
        fprintf(stderr, "Failed to determine encrypted key length\n");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_encrypt(ctx, encrypted_key, &encrypted_key_len, aes_key, AES_KEY_SIZE) <= 0) {
        fprintf(stderr, "Failed to encrypt AES key\n");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    EVP_PKEY_CTX_free(ctx);
    return (int)encrypted_key_len;
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

unsigned char *encrypt_json_with_aes(const unsigned char *aes_key, const unsigned char *iv, cJSON *json, size_t *out_len) {
    if (!aes_key || !iv || !json || !out_len) {
        fprintf(stderr, "Invalid argument(s) to encrypt_json_with_aes\n");
        return NULL;
    }

    char *json_string = cJSON_PrintUnformatted(json);
    if (!json_string) {
        fprintf(stderr, "Failed to convert JSON to string\n");
        return NULL;
    }
    
    printf("JSON string to encrypt: %s\n", json_string);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "Failed to create EVP_CIPHER_CTX\n");
        free(json_string);
        return NULL;
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, iv) != 1) {
        fprintf(stderr, "EVP_EncryptInit_ex failed\n");
        EVP_CIPHER_CTX_free(ctx);
        free(json_string);
        return NULL;
    }

    size_t json_len = strlen(json_string);
    size_t encrypted_data_len = json_len + AES_KEY_SIZE;
    unsigned char *encrypted_data = (unsigned char *)malloc(encrypted_data_len);
    if (!encrypted_data) {
        fprintf(stderr, "Failed to allocate memory for encrypted data\n");
        EVP_CIPHER_CTX_free(ctx);
        free(json_string);
        return NULL;
    }

    int len = 0;
    int total_len = 0;
    if (EVP_EncryptUpdate(ctx, encrypted_data, &len, (unsigned char *)json_string, json_len) != 1) {
        fprintf(stderr, "EVP_EncryptUpdate failed\n");
        EVP_CIPHER_CTX_free(ctx);
        free(json_string);
        free(encrypted_data);
        return NULL;
    }
    total_len = len;

    if (EVP_EncryptFinal_ex(ctx, encrypted_data + len, &len) != 1) {
        fprintf(stderr, "EVP_EncryptFinal_ex failed\n");
        EVP_CIPHER_CTX_free(ctx);
        free(json_string);
        free(encrypted_data);
        return NULL;
    }
    total_len += len;

    EVP_CIPHER_CTX_free(ctx);
    free(json_string);

    *out_len = total_len;
    return encrypted_data;
}

