#include "../inc/client.h"

int generate_aes_key_iv(unsigned char *key, unsigned char *iv) {
    if (!RAND_bytes(key, AES_KEY_SIZE) || !RAND_bytes(iv, AES_IV_SIZE)) {
        fprintf(stderr, "Failed to generate AES key or IV\n");
        return -1;
    }
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
//
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

