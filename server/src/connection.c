#include "../inc/server.h"
/*
int mx_receive_aes(t_client *client, unsigned char *encrypted_aes_key, size_t *encrypted_key_len, unsigned char *iv) {
    if (!client || !encrypted_aes_key || !encrypted_key_len || !iv) {
        syslog(LOG_ERR, "Error: Invalid arguments passed to mx_receive_aes");
        return -1;
    }

    char buffer[1024];
    ssize_t received = recv(client->socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (received <= 0) {
        syslog(LOG_ERR, "Failed to receive data from Client id: %ld", client->thread_id);
        return -1;
    }
    buffer[received] = '\0';

    cJSON *json_payload = cJSON_Parse(buffer);
    if (!json_payload) {
        syslog(LOG_ERR, "Failed to parse JSON from Client id: %ld", client->thread_id);
        return -1;
    }

    cJSON *aes_key_hex_json = cJSON_GetObjectItem(json_payload, "aes_key");
    cJSON *iv_hex_json = cJSON_GetObjectItem(json_payload, "iv");
    if (!aes_key_hex_json || !iv_hex_json || 
        !cJSON_IsString(aes_key_hex_json) || !cJSON_IsString(iv_hex_json)) {
        syslog(LOG_ERR, "Invalid JSON structure from Client id: %ld", client->thread_id);
        cJSON_Delete(json_payload);
        return -1;
    }

    int encrypted_len = mx_hex_to_bytes(aes_key_hex_json->valuestring, encrypted_aes_key, 256);
    if (encrypted_len <= 0) {
        syslog(LOG_ERR, "Failed to decode encrypted AES key from Client id: %ld", client->thread_id);
        cJSON_Delete(json_payload);
        return -1;
    }

    *encrypted_key_len = (size_t)encrypted_len;

    int iv_len = mx_hex_to_bytes(iv_hex_json->valuestring, iv, AES_IV_SIZE);
    if (iv_len != AES_IV_SIZE) {
        syslog(LOG_ERR, "Failed to decode IV from Client id: %ld", client->thread_id);
        cJSON_Delete(json_payload);
        return -1;
    }

    syslog(LOG_INFO, "Successfully received encrypted AES key and IV from Client id: %ld", client->thread_id);
    cJSON_Delete(json_payload);
    return 0;
}
*/
int mx_receive_aes(t_client *client, unsigned char *encrypted_aes_key, size_t *encrypted_key_len, unsigned char *iv) {
    if (!client || !encrypted_aes_key || !encrypted_key_len || !iv) {
        syslog(LOG_ERR, "Error: Invalid arguments passed to mx_receive_aes");
        return -1;
    }

    char buffer[1024];
    ssize_t received = recv(client->socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (received <= 0) {
        syslog(LOG_ERR, "Failed to receive data from Client id: %ld", client->thread_id);
        return -1;
    }
    buffer[received] = '\0';

    cJSON *json_payload = cJSON_Parse(buffer);
    if (!json_payload) {
        syslog(LOG_ERR, "Failed to parse JSON from Client id: %ld", client->thread_id);
        return -1;
    }

    cJSON *aes_key_b64_json = cJSON_GetObjectItem(json_payload, "aes_key");
    cJSON *iv_b64_json = cJSON_GetObjectItem(json_payload, "iv");
    if (!aes_key_b64_json || !iv_b64_json || 
        !cJSON_IsString(aes_key_b64_json) || !cJSON_IsString(iv_b64_json)) {
        syslog(LOG_ERR, "Invalid JSON structure from Client id: %ld", client->thread_id);
        cJSON_Delete(json_payload);
        return -1;
    }

    unsigned char *decoded_aes_key = base64_decode(aes_key_b64_json->valuestring, encrypted_key_len);
    if (!decoded_aes_key || *encrypted_key_len == 0 || *encrypted_key_len > 256) {
        syslog(LOG_ERR, "Failed to decode encrypted AES key from Client id: %ld", client->thread_id);
        cJSON_Delete(json_payload);
        if (decoded_aes_key) free(decoded_aes_key);
        return -1;
    }
    memcpy(encrypted_aes_key, decoded_aes_key, *encrypted_key_len);
    free(decoded_aes_key);

    size_t iv_len = 0;
    unsigned char *decoded_iv = base64_decode(iv_b64_json->valuestring, &iv_len);
    if (!decoded_iv || iv_len != AES_IV_SIZE) {
        syslog(LOG_ERR, "Failed to decode IV from Client id: %ld", client->thread_id);
        cJSON_Delete(json_payload);
        if (decoded_iv) free(decoded_iv);
        return -1;
    }
    memcpy(iv, decoded_iv, AES_IV_SIZE);
    free(decoded_iv);

    syslog(LOG_INFO, "Successfully received encrypted AES key and IV from Client id: %ld", client->thread_id);
    cJSON_Delete(json_payload);
    return 0;
}
