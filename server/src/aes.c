#include "../inc/server.h"

int decrypt_aes_key_with_private_key(const unsigned char *encrypted_aes_key, int encrypted_len, unsigned char *aes_key) {
    FILE *key_file = fopen("private_key.pem", "r");
    if (!key_file) {
        syslog(LOG_ERR, "Failed to open private key file");
        return -1;
    }

    EVP_PKEY *private_key = PEM_read_PrivateKey(key_file, NULL, NULL, NULL);
    fclose(key_file);
    if (!private_key) {
        syslog(LOG_ERR, "Failed to load private key from PEM file");
        return -1;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(private_key, NULL);
    if (!ctx) {
        syslog(LOG_ERR, "Failed to create EVP_PKEY_CTX");
        EVP_PKEY_free(private_key);
        return -1;
    }

    if (EVP_PKEY_decrypt_init(ctx) <= 0) {
        syslog(LOG_ERR, "EVP_PKEY_decrypt_init failed");
        EVP_PKEY_free(private_key);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    size_t aes_key_len = 0;
    if (EVP_PKEY_decrypt(ctx, NULL, &aes_key_len, encrypted_aes_key, encrypted_len) <= 0) {
        syslog(LOG_ERR, "EVP_PKEY_decrypt failed to determine AES key length");
        EVP_PKEY_free(private_key);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_decrypt(ctx, aes_key, &aes_key_len, encrypted_aes_key, encrypted_len) <= 0) {
        syslog(LOG_ERR, "EVP_PKEY_decrypt failed to decrypt AES key");
        EVP_PKEY_free(private_key);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    EVP_PKEY_free(private_key);
    EVP_PKEY_CTX_free(ctx);

    return (int)aes_key_len;
}

int aes_decrypt(const unsigned char *encrypted_data, int encrypted_len, const unsigned char *aes_key,
                const unsigned char *iv, unsigned char *decrypted_data) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return -1;
    
    int len;
    int plaintext_len = 0;

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    if (EVP_DecryptUpdate(ctx, decrypted_data, &len, encrypted_data, encrypted_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, decrypted_data + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}
