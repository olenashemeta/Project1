#include "../inc/server.h"

int mx_send_pubkey(void *arg) {
    t_client *client = (t_client *)arg;

    FILE *pubkey_file = fopen("public_key.pem", "r");
    if (!pubkey_file) {
        syslog(LOG_INFO, "Unable to open public key file");
        close(client->socket_fd);
        free(client);
        return 1;
    }

    EVP_PKEY *pubkey = PEM_read_PUBKEY(pubkey_file, NULL, NULL, NULL);
    fclose(pubkey_file);

    if (!pubkey) {
        ERR_print_errors_fp(stderr);
        close(client->socket_fd);
        free(client);
        return 1;
    }

    BIO *bio = BIO_new(BIO_s_mem());
    if (!bio) {
        syslog(LOG_INFO, "Unable to create BIO");
        EVP_PKEY_free(pubkey);
        close(client->socket_fd);
        free(client);
        return 1;
    }

    if (!PEM_write_bio_PUBKEY(bio, pubkey)) {
        ERR_print_errors_fp(stderr);
        BIO_free(bio);
        EVP_PKEY_free(pubkey);
        close(client->socket_fd);
        free(client);
        return 1;
    }

    char *pem_data;
    long pem_length = BIO_get_mem_data(bio, &pem_data);

    ssize_t sent = send(client->socket_fd, pem_data, pem_length, 0);
    if (sent == -1) {
        syslog(LOG_INFO, "Failed to send public key");
    } else if (sent != pem_length) {
        syslog(LOG_INFO, "Sent public key to client ID: %d", client->client_id);
    }

    BIO_free(bio);
    EVP_PKEY_free(pubkey);
    return 0;
}

int mx_recieve_aes(void *arg) {
    t_client *client = (t_client *)arg;
    char buffer[4096];

    ssize_t received = recv(client->socket_fd, buffer, sizeof(buffer) - 1, 0);

    if (received <= 0) {
        syslog(LOG_ERR, "Failed to receive AES key from Client id: %d", client->client_id);
        return 1;
    }
    buffer[received] = '\0';

    cJSON *json_payload = cJSON_Parse(buffer);
    if (!json_payload) {
        syslog(LOG_ERR, "Failed to parse JSON from Client id: %d", client->client_id);
        return 1;
    }

    cJSON *aes_key_hex_json = cJSON_GetObjectItem(json_payload, "aes_key");
    cJSON *iv_hex_json = cJSON_GetObjectItem(json_payload, "iv");

    if (!aes_key_hex_json || !iv_hex_json) {
        syslog(LOG_ERR, "Missing AES key or IV in JSON from Client id: %d", client->client_id);
        cJSON_Delete(json_payload);
        return 1;
    }

    if (!cJSON_IsString(aes_key_hex_json) || !cJSON_IsString(iv_hex_json)) {
        syslog(LOG_ERR, "Invalid JSON structure from Client id: %d", client->client_id);
        cJSON_Delete(json_payload);
        return 1;
    }

    unsigned char encrypted_aes_key[256];
    int aes_key_len = mx_hex_to_bytes(aes_key_hex_json->valuestring, encrypted_aes_key, sizeof(encrypted_aes_key));
    if (aes_key_len == -1) {
        syslog(LOG_ERR, "Failed to decode hex AES key from Client id: %d", client->client_id);
        cJSON_Delete(json_payload);
        return 1;
    }

    unsigned char *aes_key = (unsigned char *)malloc(AES_KEY_SIZE);
    if (decrypt_aes_key_with_private_key(encrypted_aes_key, aes_key_len, aes_key) <= 0) {
        syslog(LOG_ERR, "Failed to decrypt AES key from Client id: %d", client->client_id);
        free(aes_key);
        cJSON_Delete(json_payload);
        return 1;
    }

    char key_str[AES_KEY_SIZE * 2 + 1];
    bytes_to_hex_string(aes_key, AES_KEY_SIZE, key_str);
    syslog(LOG_INFO, "Decrypted AES Key: %s", key_str);

    unsigned char iv[AES_IV_SIZE];
    int iv_len = mx_hex_to_bytes(iv_hex_json->valuestring, iv, AES_IV_SIZE);
    if (iv_len <= 0) {
        syslog(LOG_ERR, "Failed to decode hex IV from Client id: %d", client->client_id);
        free(aes_key);
        cJSON_Delete(json_payload);
        return 1;
    }

    memcpy(client->aes_key, aes_key, AES_KEY_SIZE);
    memcpy(client->aes_iv, iv, AES_IV_SIZE);

    syslog(LOG_INFO, "Success: AES Key and IV successfully stored for Client id: %d", client->client_id);

    free(aes_key);
    cJSON_Delete(json_payload);
    return 0;
}
