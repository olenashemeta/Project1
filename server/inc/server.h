#ifndef SERVER_H
#define SERVER_H

#include "../../libs/libmx/inc/libmx.h"
#include "../../libs/JsonLib/cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>

#define AES_KEY_SIZE 32
#define AES_IV_SIZE 16 

typedef struct s_client {
    int client_id;
    int socket_fd;
    struct sockaddr_in address;
}              t_client;

void mx_daemon_start(void);
void mx_daemon_end(int signal);
void set_signal(void);
void mx_process_client_request(cJSON *json);
void handle_login_request(cJSON *json);

int aes_decrypt(const unsigned char *encrypted_data, int encrypted_len, const unsigned char *aes_key,
                const unsigned char *iv, unsigned char *decrypted_data);
int decrypt_aes_key_with_private_key(const unsigned char *encrypted_aes_key, int encrypted_len, unsigned char *aes_key);
int mx_hex_to_bytes(const char *hex_str, unsigned char *out_bytes, size_t max_bytes);

#endif 