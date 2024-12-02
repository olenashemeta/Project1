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
#include <openssl/evp.h>
#include <openssl/aes.h>

#define AES_KEY_SIZE 32
#define AES_IV_SIZE 16 

typedef struct s_keys {
	EVP_PKEY *pkey;
	unsigned char aes_key[AES_KEY_SIZE];
	unsigned char aes_iv[AES_IV_SIZE];
}				t_keys;

typedef struct s_packet {
	size_t len;
	char *data;
}			   t_packet;

typedef struct s_main {
	int socket;
	char *address;
	int rec_delay;
	int port;
	bool is_connected;
	bool is_closing;
	//cJSON *server_response;
	pthread_mutex_t lock;
	pthread_cond_t cond;
	//bool has_new_data;
	t_keys keys;

}				t_main;

typedef struct s_user {
	char* login;
	char* password;
} t_user;

//func to communicate with the server
int mx_connect_to_server(t_main *main);
int mx_receiving_pubkey(t_main *main);
int mx_transfer_aes_key(t_main *main);
int handshake(t_main *main);
void process_response(t_packet *recieved_data);

//security func
int generate_aes_key_iv(t_main *main);

unsigned char *encrypt_json_with_aes(const unsigned char *aes_key, const unsigned char *iv, 
                                     cJSON *json, size_t *out_len);
int encrypt_aes_key(EVP_PKEY *pubkey, const unsigned char *aes_key, unsigned char *encrypted_key);

int decrypt_received_data(t_packet *data, const unsigned char *aes_key, const unsigned char *iv);

//UI func
void login_window(GtkApplication *app, gpointer user_data);

//main data func
t_main *mx_create_main_data(const char *address, int port);
void mx_free_main_data(t_main *main);

//func json request
cJSON *form_login_request(const char *login, const char *password);
cJSON *form_aes_key_transfer(const unsigned char *aes_key, const unsigned char *iv, EVP_PKEY *pubkey);


//Base64
char *base64_encode(const unsigned char *input, size_t input_len);
unsigned char *base64_decode(const char *input, size_t *output_len);

//request utils func
t_packet *create_message(const char *data, size_t data_len);
void free_message(t_packet *req);
void prepare_and_send_json(cJSON *json_payload, t_main *main);
void send_message(t_packet *reg, int socket);

//receiving a response from the server
t_packet *receive_message(int socket_fd);
t_packet *create_receive(int len, const char *data);

//test func
t_user *mx_create_client(void);
void mx_print_client(const t_user *client);
void mx_free_client(t_user *client);
void test_base64_encoding(t_main *main_data);


#endif

