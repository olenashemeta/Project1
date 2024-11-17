#ifndef CLIENT_H
#define CLIENT_H

#include "../../libs/libmx/inc/libmx.h"
#include "../../libs/JsonLib/cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <gtk/gtk.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

#define AES_KEY_SIZE 32
#define AES_IV_SIZE 16 

typedef struct s_main {
	int socket;
	char *address;
	int rec_delay;
	int port;
	bool is_connected;
	bool is_closing;

	EVP_PKEY *pubkey;
	cJSON *server_response;

	pthread_mutex_t lock;
	pthread_cond_t cond;
	bool has_new_data;

	unsigned char aes_key[AES_KEY_SIZE];
    unsigned char aes_iv[AES_IV_SIZE];
}				t_main;

typedef struct s_user {
	char* login;
	char* password;
} t_user;

//func to communicate with the server
int mx_connect_to_server(t_main *main);
int mx_receiving_pubkey(int socket_fd, EVP_PKEY **pubkey);
int mx_transfer_aes_key(const unsigned char *aes_key, const unsigned char *iv, int sock, EVP_PKEY *pubkey);

//security func
int generate_aes_key_iv(unsigned char *key, unsigned char *iv);
int aes_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
unsigned char *encrypt_json_with_aes(const unsigned char *aes_key, const unsigned char *iv, cJSON *json, size_t *out_len);
int aes_decrypt(const unsigned char *ciphertext, int ciphertext_len, const unsigned char *key, const unsigned char *iv, unsigned char *plaintext);
int encrypt_aes_key(EVP_PKEY *pubkey, const unsigned char *aes_key, unsigned char *encrypted_key);

//UI func
void login_window(GtkApplication *app, gpointer user_data);

//main data func
t_main *mx_create_main_data(const char *address, int port);
void mx_free_main_data(t_main *main);

//func json request
cJSON *form_login_request_test(const char *login, const char *password);
cJSON *form_aes_key_transfer(const unsigned char *aes_key, const unsigned char *iv, EVP_PKEY *pubkey);

//hex utils
void bytes_to_hex_string(const unsigned char *bytes, int len, char *hex_str);
int mx_hex_to_bytes(const char *hex_str, unsigned char *out_bytes, size_t max_bytes);

//test func
t_user *mx_create_client(void);
void mx_print_client(const t_user *client);
void mx_free_client(t_user *client);

#endif