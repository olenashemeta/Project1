#include "../inc/client.h"

void bytes_to_hex_string(const unsigned char *bytes, int len, char *hex_str) {
    for (int i = 0; i < len; i++) {
        sprintf(hex_str + (i * 2), "%02x", bytes[i]);
    }
    hex_str[len * 2] = '\0';
}

cJSON *form_login_request(const char *login, const unsigned char *encrypted_password, int encrypted_password_len,
                          const unsigned char *encrypted_aes_key, int encrypted_aes_key_len,
                          const unsigned char *iv) {
    cJSON *json = cJSON_CreateObject();
    if (!json) {
        printf("Error creating JSON object\n");
        return NULL;
    }

    cJSON_AddStringToObject(json, "request_type", "login");
    cJSON_AddStringToObject(json, "userlogin", login);

    char encrypted_password_hex[encrypted_password_len * 2 + 1];
    char encrypted_aes_key_hex[encrypted_aes_key_len * 2 + 1];
    char iv_hex[AES_IV_SIZE * 2 + 1];

    bytes_to_hex_string(encrypted_password, encrypted_password_len, encrypted_password_hex);
    bytes_to_hex_string(encrypted_aes_key, encrypted_aes_key_len, encrypted_aes_key_hex);
    bytes_to_hex_string(iv, AES_IV_SIZE, iv_hex);

    cJSON_AddStringToObject(json, "password", encrypted_password_hex);
    cJSON_AddStringToObject(json, "encrypted_aes_key", encrypted_aes_key_hex);
    cJSON_AddStringToObject(json, "iv", iv_hex);

    return json;
}
