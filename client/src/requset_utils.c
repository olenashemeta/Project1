#include "../inc/client.h"

t_request *create_request(const char *data) {
    if (!data) {
        return NULL;
    }

    t_request *request = malloc(sizeof(t_request));
    if (!request) {
        return NULL;
    }

    request->len = strlen(data);
    request->data = malloc(request->len);
    if (!request->data) {
        free(request);
        return NULL;
    }

    strcpy(request->data, data);
    return request;
}

void free_request(t_request *req) {
    if (req) {
        free(req->data);
        free(req);
    }
}

void send_request(t_request *req, int socket) {
    if (!req)
        return;

    if (send(socket, &req->len, sizeof(req->len), 0) == -1 || send(socket, &req->data, req->len, 0) == -1)
        return;
}
