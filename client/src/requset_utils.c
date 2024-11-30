#include "../inc/client.h"

t_packet *create_request(const char *data, size_t data_len) {
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

void free_request(t_packet *req) {
    if (req) {
        if (req->data) {
            free(req->data);
            req->data = NULL;
        }
        free(req);
    }
}

void send_request(t_packet *req, int socket) {
    if (!req) {
        return;
    }

    if (send(socket, &req->len, sizeof(req->len), 0) == -1 || send(socket, req->data, req->len, 0) == -1)
        return;
}
