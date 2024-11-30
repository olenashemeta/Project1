#include "../inc/client.h"

t_packet *receive_message(int socket_fd) {
    size_t len;

    ssize_t rec_data = recv(socket_fd, &len, sizeof(len), 0);
    if (rec_data <= 0){
        return NULL;
    }

    t_packet *data = create_message(NULL, len);
    if (!data) {
        return NULL;
    }

    rec_data = recv(socket_fd, data->data, data->len, 0);
    if (rec_data <= 0) {
        free_message(data);
        return NULL;
    }

    return data;
}

t_packet *create_message(const char *data, size_t data_len) {
    if (!data || data_len == 0) {
        return NULL;
    }

    t_packet *request = malloc(sizeof(t_packet));
    if (!request) {
        return NULL;
    }

    request->len = data_len;
    request->data = malloc(request->len);
    if (!request->data) {
        free(request);
        return NULL;
    }

    memcpy(request->data, data, data_len);
    return request;
}

void free_message(t_packet *data) {
    if (data) {
        if (data->data) {
            free(data->data);
            data->data = NULL;
        }
        free(data);
    }
}

void send_message(t_packet *req, int socket) {
    if (!req) {
        return;
    }

    if (send(socket, &req->len, sizeof(req->len), 0) == -1 || send(socket, req->data, req->len, 0) == -1)
        return;
}


void prepare_and_send_json(cJSON *json_payload, t_main *main) {
    if (!main || !json_payload) {
        fprintf(stderr, "Invalid arguments to prepare_and_send_json\n");
        return;
    }

    size_t encrypted_data_len;
    unsigned char *encrypted_data = encrypt_json_with_aes(main->keys.aes_key, main->keys.aes_iv, json_payload, &encrypted_data_len);
    if (!encrypted_data) {
        fprintf(stderr, "Failed to encrypt JSON object\n");
        cJSON_Delete(json_payload);
        return;
    }

    if (main->is_connected) {
        t_packet *req = create_message((char *)encrypted_data, encrypted_data_len);
        if (req) {
            send_message(req, main->socket);
            free_message(req);
        }
    }

    cJSON_Delete(json_payload);
    free(encrypted_data);
}
