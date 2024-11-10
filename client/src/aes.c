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

int encrypt_aes_key(EVP_PKEY *pubkey, unsigned char *aes_key, unsigned char *encrypted_key) {
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


/*
// Функція для кодування в Base64
char* base64_encode(const unsigned char *input, int length) {
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO *bio = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bio);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BUF_MEM *buffer_ptr;
    BIO_get_mem_ptr(b64, &buffer_ptr);
    char *encoded_data = (char *)malloc(buffer_ptr->length + 1);
    memcpy(encoded_data, buffer_ptr->data, buffer_ptr->length);
    encoded_data[buffer_ptr->length] = '\0';
    BIO_free_all(b64);
    return encoded_data;
}

// Функція шифрування AES-алгоритмом
int aes_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    AES_KEY encrypt_key;
    AES_set_encrypt_key(key, 128, &encrypt_key);
    AES_cbc_encrypt(plaintext, ciphertext, plaintext_len, &encrypt_key, iv, AES_ENCRYPT);
    return plaintext_len;
}

int aes_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    AES_KEY decrypt_key;
    AES_set_decrypt_key(key, 128, &decrypt_key);
    AES_cbc_encrypt(ciphertext, plaintext, ciphertext_len, &decrypt_key, iv, AES_DECRYPT);
    return ciphertext_len; 
}
*/

