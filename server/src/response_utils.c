#include "../inc/server.h"

t_receive *receive_request(int socket_fd) {
    size_t len;

    ssize_t rec_data = recv(socket_fd, &len, sizeof(len), 0);
    if (rec_data <= 0){
        return NULL;
    }

    t_receive *data = create_receive(len, NULL);
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

t_receive *create_receive(int len, const char *data) {
    t_receive *receive = (t_receive *)malloc(sizeof(t_receive));
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

void free_receive(t_receive *receive) {
    if (receive) {
        if (receive->data) {
            free(receive->data);
            receive->data = NULL;
        }
        free(receive);
    }
}

