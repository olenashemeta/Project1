#include "../inc/server.h"

int generate_rsa_keys(t_keys *keys) {
    if (!keys) {
        syslog(LOG_ERR, "Error: keys structure is NULL");
        return -1;
    }

    EVP_PKEY_CTX *ctx = NULL;
    keys->pkey = NULL;
    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx) {
        syslog(LOG_ERR, "Error: EVP_PKEY_CTX_new_id failed");
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        syslog(LOG_ERR, "Error: EVP_PKEY_keygen_init failed");
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, OPENSSL_RSA_FIPS_MIN_MODULUS_BITS) <= 0) {
        syslog(LOG_ERR, "Error: EVP_PKEY_CTX_set_rsa_keygen_bits failed");
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_keygen(ctx, &keys->pkey) <= 0) {
        syslog(LOG_ERR, "Error: EVP_PKEY_keygen failed");
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    syslog(LOG_INFO, "RSA keys successfully generated and stored in t_keys");
    EVP_PKEY_CTX_free(ctx);
    return 0;
}

void free_keys(t_keys *keys) {
    if (!keys) return;

    if (keys->pkey) {
        EVP_PKEY_free(keys->pkey);
        keys->pkey = NULL;
    }
}

int handshake(t_client *client) {
    if (!client) {
        syslog(LOG_ERR, "Error: client is NULL");
        return -1;
    }

    if (generate_rsa_keys(&client->keys) != 0) {
        syslog(LOG_ERR, "Error: Failed to generate RSA keys for handshake");
        return -1;
    }

    unsigned char *pubkey_pem = NULL;
    size_t pubkey_len = 0;
    unsigned char *privkey_pem = NULL;
    size_t privkey_len = 0;

    if (rsa_keys_to_pem(client->keys.pkey, &pubkey_pem, &pubkey_len, &privkey_pem, &privkey_len) != 0) {
        syslog(LOG_ERR, "Error: Failed to export keys to PEM format");
        free_client_keys(client);
        return -1;
    }


    if (send(client->socket_fd, pubkey_pem, pubkey_len, 0) != (ssize_t)pubkey_len) {
        syslog(LOG_ERR, "Error: Failed to send public key to client");
        free(pubkey_pem);
        free(privkey_pem);
        free_client_keys(client);
        return -1;
    }
    free(pubkey_pem);

    unsigned char encrypted_aes_key[256];
    size_t encrypted_key_len = 0;
    unsigned char iv[AES_IV_SIZE];

    if (mx_receive_aes(client, encrypted_aes_key, &encrypted_key_len, iv) != 0) {
        syslog(LOG_ERR, "Error: Failed to receive encrypted AES key and IV from client");
        free(privkey_pem);
        free_client_keys(client);
        return -1;
    }

    unsigned char *decrypted_key = NULL;
    size_t decrypted_key_len = 0;
    EVP_PKEY *private_key = PEM_read_bio_PrivateKey(BIO_new_mem_buf(privkey_pem, privkey_len), NULL, NULL, NULL);
    free(privkey_pem);

    if (!private_key || rsa_decrypt_aes_key(private_key, encrypted_aes_key, encrypted_key_len, 
                                            &decrypted_key, &decrypted_key_len) != 0) {
        syslog(LOG_ERR, "Error: Failed to decrypt AES key");
        if (private_key) EVP_PKEY_free(private_key);
        free_client_keys(client);
        return -1;
    }
    EVP_PKEY_free(private_key);

    if (decrypted_key_len != AES_KEY_SIZE) {
        syslog(LOG_ERR, "Error: Invalid decrypted AES key length");
        free(decrypted_key);
        free_client_keys(client);
        return -1;
    }

    memcpy(client->keys.aes_key, decrypted_key, AES_KEY_SIZE);
    memcpy(client->keys.aes_iv, iv, AES_IV_SIZE);
    free(decrypted_key);

    syslog(LOG_INFO, "Handshake successful: AES key and IV stored for client");

    return 0;
}

int rsa_keys_to_pem(EVP_PKEY *pkey, unsigned char **pubkey_pem, size_t *pubkey_len,
                    unsigned char **privkey_pem, size_t *privkey_len) {
    if (!pkey || !pubkey_pem || !pubkey_len || !privkey_pem || !privkey_len) {
        syslog(LOG_ERR, "Error: Invalid parameters for rsa_keys_to_pem");
        return -1;
    }

    BIO *pub_mem = BIO_new(BIO_s_mem());
    BIO *priv_mem = BIO_new(BIO_s_mem());
    if (!pub_mem || !priv_mem) {
        syslog(LOG_ERR, "Error: Failed to create BIOs for PEM export");
        if (pub_mem) BIO_free(pub_mem);
        if (priv_mem) BIO_free(priv_mem);
        return -1;
    }

    // Write public key to PEM
    if (!PEM_write_bio_PUBKEY(pub_mem, pkey)) {
        syslog(LOG_ERR, "Error: Failed to write public key to PEM");
        BIO_free(pub_mem);
        BIO_free(priv_mem);
        return -1;
    }

    // Write private key to PEM
    if (!PEM_write_bio_PrivateKey(priv_mem, pkey, NULL, NULL, 0, NULL, NULL)) {
        syslog(LOG_ERR, "Error: Failed to write private key to PEM");
        BIO_free(pub_mem);
        BIO_free(priv_mem);
        return -1;
    }

    // Extract public key PEM data
    char *pub_data;
    *pubkey_len = BIO_get_mem_data(pub_mem, &pub_data);
    *pubkey_pem = malloc(*pubkey_len);
    if (!*pubkey_pem) {
        syslog(LOG_ERR, "Error: Failed to allocate memory for public key PEM");
        BIO_free(pub_mem);
        BIO_free(priv_mem);
        return -1;
    }
    memcpy(*pubkey_pem, pub_data, *pubkey_len);

    // Extract private key PEM data
    char *priv_data;
    *privkey_len = BIO_get_mem_data(priv_mem, &priv_data);
    *privkey_pem = malloc(*privkey_len);
    if (!*privkey_pem) {
        syslog(LOG_ERR, "Error: Failed to allocate memory for private key PEM");
        free(*pubkey_pem);
        BIO_free(pub_mem);
        BIO_free(priv_mem);
        return -1;
    }
    memcpy(*privkey_pem, priv_data, *privkey_len);

    BIO_free(pub_mem);
    BIO_free(priv_mem);

    return 0;
}

int rsa_decrypt_aes_key(EVP_PKEY *private_key, const unsigned char *encrypted_aes_key, size_t encrypted_len, 
                        unsigned char **decrypted_key, size_t *decrypted_key_len) {
    if (!private_key || !encrypted_aes_key || !decrypted_key || !decrypted_key_len) {
        syslog(LOG_ERR, "Error: Invalid parameters for rsa_decrypt_aes_key");
        return -1;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(private_key, NULL);
    if (!ctx) {
        syslog(LOG_ERR, "Error: Failed to create EVP_PKEY_CTX");
        return -1;
    }

    if (EVP_PKEY_decrypt_init(ctx) <= 0) {
        syslog(LOG_ERR, "Error: EVP_PKEY_decrypt_init failed");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    size_t out_len = 0;
    if (EVP_PKEY_decrypt(ctx, NULL, &out_len, encrypted_aes_key, encrypted_len) <= 0) {
        syslog(LOG_ERR, "Error: EVP_PKEY_decrypt failed to determine output length");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    unsigned char *out = malloc(out_len);
    if (!out) {
        syslog(LOG_ERR, "Error: Failed to allocate memory for decrypted key");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_decrypt(ctx, out, &out_len, encrypted_aes_key, encrypted_len) <= 0) {
        syslog(LOG_ERR, "Error: EVP_PKEY_decrypt failed to decrypt AES key");
        free(out);
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    *decrypted_key = out;
    *decrypted_key_len = out_len;

    EVP_PKEY_CTX_free(ctx);
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

