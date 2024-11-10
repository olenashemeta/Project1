#include "../inc/server.h"

int mx_hex_to_bytes(const char *hex_str, unsigned char *out_bytes, size_t max_bytes) {
    size_t hex_len = strlen(hex_str);
    if (hex_len % 2 != 0 || hex_len / 2 > max_bytes) {
        return -1;  // Некоректна довжина або недостатньо місця
    }

    for (size_t i = 0; i < hex_len; i += 2) {
        char hex_byte[3] = { hex_str[i], hex_str[i + 1], '\0' };
        out_bytes[i / 2] = (unsigned char)strtoul(hex_byte, NULL, 16);
    }
    return hex_len / 2;
}

void handle_login_request(cJSON *json) {
    cJSON *userlogin_json = cJSON_GetObjectItemCaseSensitive(json, "userlogin");
    cJSON *encrypted_password_json = cJSON_GetObjectItemCaseSensitive(json, "password");
    cJSON *encrypted_aes_key_json = cJSON_GetObjectItemCaseSensitive(json, "encrypted_aes_key");
    cJSON *iv_json = cJSON_GetObjectItemCaseSensitive(json, "iv");

    if (!cJSON_IsString(userlogin_json) || !cJSON_IsString(encrypted_password_json) ||
        !cJSON_IsString(encrypted_aes_key_json) || !cJSON_IsString(iv_json)) {
        syslog(LOG_ERR, "Invalid login request data");
        cJSON_Delete(json);
        return;
    }

    const char *login = userlogin_json->valuestring;
    const char *encrypted_password_hex = encrypted_password_json->valuestring;
    const char *encrypted_aes_key_hex = encrypted_aes_key_json->valuestring;
    const char *iv_hex = iv_json->valuestring;

    syslog(LOG_INFO, "Received login request - Login: %s, Encrypted Password (hex): %s, Encrypted AES Key (hex): %s, IV (hex): %s",
           login, encrypted_password_hex, encrypted_aes_key_hex, iv_hex);

    unsigned char encrypted_aes_key[256];
    int aes_key_len = mx_hex_to_bytes(encrypted_aes_key_hex, encrypted_aes_key, sizeof(encrypted_aes_key));
    if (aes_key_len == -1) {
        syslog(LOG_ERR, "Invalid AES key length");
        cJSON_Delete(json);
        return;
    }

    unsigned char aes_key[AES_KEY_SIZE];
    int decrypted_aes_key_len = decrypt_aes_key_with_private_key(encrypted_aes_key, aes_key_len, aes_key);
    if (decrypted_aes_key_len == -1) {
        syslog(LOG_ERR, "Failed to decrypt AES key");
        cJSON_Delete(json);
        return;
    }

    unsigned char encrypted_password[128];
    int password_len = mx_hex_to_bytes(encrypted_password_hex, encrypted_password, sizeof(encrypted_password));
    if (password_len == -1) {
        syslog(LOG_ERR, "Invalid password length");
        cJSON_Delete(json);
        return;
    }

    unsigned char iv[AES_IV_SIZE];
    int iv_len = mx_hex_to_bytes(iv_hex, iv, sizeof(iv));
    if (iv_len == -1) {
        syslog(LOG_ERR, "Invalid IV length");
        cJSON_Delete(json);
        return;
    }

    unsigned char decrypted_password[128];
    int decrypted_len = aes_decrypt(encrypted_password, password_len, aes_key, iv, decrypted_password);
    if (decrypted_len == -1) {
        syslog(LOG_ERR, "Password decryption failed");
        cJSON_Delete(json);
        return;
    }
    decrypted_password[decrypted_len] = '\0';

    syslog(LOG_INFO, "Login attempt - Login: %s, Password: %s", login, decrypted_password);

    cJSON_Delete(json);
}
