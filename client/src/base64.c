#include "../inc/client.h"

char *base64_encode(const unsigned char *input, size_t input_len) {
    BIO *bio, *b64;
    BUF_MEM *buffer_ptr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bio);

    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    BIO_write(b64, input, input_len);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &buffer_ptr);

    // Создаем копию строки Base64
    char *base64_str = (char *)malloc(buffer_ptr->length + 1);
    if (!base64_str) {
        BIO_free_all(b64);
        return NULL;
    }

    memcpy(base64_str, buffer_ptr->data, buffer_ptr->length);
    base64_str[buffer_ptr->length] = '\0';

    BIO_free_all(b64);
    return base64_str;
}

unsigned char *base64_decode(const char *input, size_t *output_len) {
    BIO *bio, *b64;
    int input_len = strlen(input);

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(input, input_len);
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    unsigned char *buffer = (unsigned char *)malloc(input_len);
    if (!buffer) {
        BIO_free_all(bio);
        return NULL;
    }

    *output_len = BIO_read(bio, buffer, input_len);
    if (*output_len <= 0) {
        free(buffer);
        buffer = NULL;
    }

    BIO_free_all(bio);
    return buffer;
}

void test_base64_encoding(t_main *main_data) {
    if (!main_data || !main_data->keys.pkey) {
        printf("Invalid main data or missing public key\n");
        return;
    }

    // 1. Вывод оригинального AES-ключа
    printf("Original AES key: ");
    for (int i = 0; i < AES_KEY_SIZE; i++) {
        printf("%02X", main_data->keys.aes_key[i]);
    }
    printf("\n");

    // 2. Шифрование AES-ключа открытым ключом RSA
    unsigned char encrypted_key[256]; // Буфер для зашифрованного ключа
    int encrypted_key_len = encrypt_aes_key(main_data->keys.pkey, main_data->keys.aes_key, encrypted_key);

    if (encrypted_key_len == -1) {
        printf("Error encrypting AES key: %s\n", ERR_error_string(ERR_get_error(), NULL));
        return;
    }

    printf("Encrypted AES key (raw): ");
    for (int i = 0; i < encrypted_key_len; i++) {
        printf("%02X", encrypted_key[i]);
    }
    printf("\n");

    // 3. Кодирование зашифрованного ключа в Base64
    char *encoded_key = base64_encode(encrypted_key, encrypted_key_len);
    if (!encoded_key) {
        printf("Failed to encode AES key to Base64\n");
        return;
    }
    printf("Base64-encoded AES key: %s\n", encoded_key);

    // 4. Декодирование ключа из Base64
    size_t decoded_key_len;
    unsigned char *decoded_key = base64_decode(encoded_key, &decoded_key_len);
    if (!decoded_key) {
        printf("Failed to decode AES key from Base64\n");
        free(encoded_key);
        return;
    }

    printf("Decoded AES key (raw): ");
    for (size_t i = 0; i < decoded_key_len; i++) {
        printf("%02X", decoded_key[i]);
    }
    printf("\n");


    // Освобождение памяти
    free(encoded_key);
    free(decoded_key);
}
