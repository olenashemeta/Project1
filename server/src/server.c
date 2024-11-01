#include "../inc/server.h"

int main(void) {
    int server_fd, server_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char buffer[1024] = {0};
    char *response = "Hello from server";

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;            // IPv4
    address.sin_addr.s_addr = INADDR_ANY;     // Принимать подключения от любого IP
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8080\n");

    server_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (server_socket < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted\n");

    read(server_socket, buffer, 1024);
    printf("Received from client: %s\n", buffer);

    send(server_socket, response, strlen(response), 0);
    printf("Response sent to client\n");

    while(1) {

    }

    close(server_socket);
    close(server_fd);
    return 0;
}
