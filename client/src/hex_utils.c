#include "../inc/client.h"

void bytes_to_hex_string(const unsigned char *bytes, int len, char *hex_str) {
    for (int i = 0; i < len; i++) {
        sprintf(hex_str + (i * 2), "%02x", bytes[i]);
    }
    hex_str[len * 2] = '\0';
}

int mx_hex_to_bytes(const char *hex_str, unsigned char *out_bytes, size_t max_bytes) {
    size_t hex_len = strlen(hex_str);
    if (hex_len % 2 != 0 || hex_len / 2 > max_bytes) {
        return -1;
    }

    for (size_t i = 0; i < hex_len; i += 2) {
        char hex_byte[3] = { hex_str[i], hex_str[i + 1], '\0' };
        out_bytes[i / 2] = (unsigned char)strtoul(hex_byte, NULL, 16);
    }
    return hex_len / 2;
}

void test_base64(const unsigned char *aes_key) {
    printf("Key before:");
    for (int i = 0; i < AES_KEY_SIZE; i++) {
        printf("%02x", aes_key[i]);
    }
    printf("\n");

    char hex_str[AES_KEY_SIZE * 2 + 1];
    bytes_to_hex_string(aes_key, AES_KEY_SIZE, hex_str);

    unsigned char decoded[AES_KEY_SIZE];
    int decoded_len = mx_hex_to_bytes(hex_str, decoded, AES_KEY_SIZE);

    printf("Key after:");
    for (int i = 0; i < AES_KEY_SIZE; i++) {
        printf("%02x", decoded[i]);
    }
    printf("\n");
}

