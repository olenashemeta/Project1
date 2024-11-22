#include "../inc/server.h"

// Функція для звільнення ресурсів
static void cleanup_resources(EVP_PKEY_CTX *ctx, EVP_PKEY *pkey, FILE *private_file, FILE *public_file) {
    if (ctx) EVP_PKEY_CTX_free(ctx);
    if (pkey) EVP_PKEY_free(pkey);
    if (private_file) fclose(private_file);
    if (public_file) fclose(public_file);
}

// Функція для генерації RSA-ключів
int generate_rsa_keys(void) {
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_CTX *ctx = NULL;
    FILE *private_file = NULL, *public_file = NULL;

    // Ініціалізація контексту для генерації ключа
    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx) {
        syslog(LOG_ERR, "Error: EVP_PKEY_CTX_new_id failed");
        ERR_print_errors_fp(stderr);
        cleanup_resources(ctx, pkey, private_file, public_file);
        return -1;
    }

    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        syslog(LOG_ERR, "Error: EVP_PKEY_keygen_init failed");
        ERR_print_errors_fp(stderr);
        cleanup_resources(ctx, pkey, private_file, public_file);
        return -1;
    }

    // Встановлення розміру ключа
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, OPENSSL_RSA_FIPS_MIN_MODULUS_BITS) <= 0) {
        syslog(LOG_ERR, "Error: EVP_PKEY_CTX_set_rsa_keygen_bits failed");
        ERR_print_errors_fp(stderr);
        cleanup_resources(ctx, pkey, private_file, public_file);
        return -1;
    }

    // Генерація ключа
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        syslog(LOG_ERR, "Error: EVP_PKEY_keygen failed");
        ERR_print_errors_fp(stderr);
        cleanup_resources(ctx, pkey, private_file, public_file);
        return -1;
    }

    // Збереження приватного ключа
    private_file = fopen("private_key.pem", "w");
    if (!private_file) {
        syslog(LOG_ERR, "Error: Unable to open file for private key");
        cleanup_resources(ctx, pkey, private_file, public_file);
        return -1;
    }
    if (!PEM_write_PrivateKey(private_file, pkey, NULL, NULL, 0, NULL, NULL)) {
        syslog(LOG_ERR, "Error: Unable to write private key");
        ERR_print_errors_fp(stderr);
        cleanup_resources(ctx, pkey, private_file, public_file);
        return -1;
    }

    // Збереження публічного ключа
    public_file = fopen("public_key.pem", "w");
    if (!public_file) {
        syslog(LOG_ERR, "Error: Unable to open file for public key");
        cleanup_resources(ctx, pkey, private_file, public_file);
        return -1;
    }
    if (!PEM_write_PUBKEY(public_file, pkey)) {
        syslog(LOG_ERR, "Error: Unable to write public key");
        ERR_print_errors_fp(stderr);
        cleanup_resources(ctx, pkey, private_file, public_file);
        return -1;
    }

    syslog(LOG_INFO, "RSA keys successfully generated and saved");
    cleanup_resources(ctx, pkey, private_file, public_file);
    return 0;
    
}

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

