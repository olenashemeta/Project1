#include "../inc/server.h"

t_packet *receive_request(int socket_fd) {
    size_t len;

    ssize_t rec_data = recv(socket_fd, &len, sizeof(len), 0);
    if (rec_data <= 0){
        return NULL;
    }

    t_packet *data = create_receive(len, NULL);
    if (!data) {
        return NULL;
    }

    rec_data = recv(socket_fd, data->data, data->len, 0);
    if (rec_data <= 0) {
        free_receive(data);
        return NULL;
    }

    return data;
}

t_packet *create_receive(int len, const char *data) {
    t_packet *receive = (t_packet *)malloc(sizeof(t_packet));
    if (!receive) {
        return NULL;
    }
    
    receive->len = len;
    receive->data = (char *)malloc(len);
    if (!receive->data) {
        free(receive);
        return NULL;
    }
    
    if (data) {
        memcpy(receive->data, data, len);
    }

    return receive;
}

void free_receive(t_packet *receive) {
    if (receive) {
        if (receive->data) {
            free(receive->data);
            receive->data = NULL;
        }
        free(receive);
    }
}

void prepare_and_send_json(cJSON *json_payload, t_client *client) {
    if (!client || !json_payload) {
        fprintf(stderr, "Invalid arguments to prepare_and_send_json\n");
        return;
    }

    size_t encrypted_data_len;
    unsigned char *encrypted_data = encrypt_json_with_aes(client->keys.aes_key, client->keys.aes_iv, json_payload, &encrypted_data_len);
    if (!encrypted_data) {
        fprintf(stderr, "Failed to encrypt JSON object\n");
        cJSON_Delete(json_payload);
        return;
    }
    /*
    if (main->is_connected) {
        t_packet *req = create_request((char *)encrypted_data, encrypted_data_len);
        if (req) {
            send_request(req, main->socket);
            free_request(req);
        }
    }
    */
    cJSON_Delete(json_payload);
    free(encrypted_data);
}


