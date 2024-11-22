#include "../inc/server.h"

volatile sig_atomic_t daemon_running = 1;

int start_server(const char *port) {

    struct addrinfo network_config, *server_info;
    memset(&network_config, 0, sizeof(network_config));

    network_config.ai_family = AF_INET;
    network_config.ai_socktype = SOCK_STREAM;
    network_config.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &network_config, &server_info) != 0) {
        perror("getaddrinfo failed");
        return -1;
    }

    int server_sd = socket(server_info->ai_family, server_info->ai_socktype, 0);
    if (server_sd == -1) {
        perror("Socket creation failed");
        freeaddrinfo(server_info);
        return -1;
    }

    if (bind(server_sd, server_info->ai_addr, server_info->ai_addrlen) == -1) {
        perror("Bind failed");
        close(server_sd);
        freeaddrinfo(server_info);
        return -1;
    }

    freeaddrinfo(server_info);

    if (listen(server_sd, 10) == -1) {
        perror("Listen failed");
        close(server_sd);
        return -1;
    }

    syslog(LOG_INFO, "Server listening on port %s", port);

    while (daemon_running) {
        struct sockaddr addr;
        socklen_t addrlen;

        int socket_fd = accept(server_sd, &addr, &addrlen);
        if (socket_fd == -1) {
            perror("Accept failed");
            continue;
        }

        t_client *client = create_new_client(socket_fd);

        char addrstr[INET_ADDRSTRLEN];
        struct sockaddr_in *client_addr = (struct sockaddr_in *)&addr;
        inet_ntop(AF_INET, &client_addr->sin_addr, addrstr, INET_ADDRSTRLEN);
        syslog(LOG_INFO, "Server IP: %s", addrstr);

        if (pthread_create(&client->thread_id, NULL, handle_client, client) != 0) {
            perror("Thread creation failed");
            free_client(client);
            continue;
        }
    }
    syslog(LOG_INFO, "Shutting down server");
    closelog();
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./uchat_server [port]\n");
        return -1;
    }

    const char *port = argv[1];
    
    mx_daemon_start();
    set_signal();

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd))) {
        syslog(LOG_INFO, "Current working directory: %s", cwd);
    } else {
        perror("getcwd failed");
    }

    if (start_server(port) == -1) {
        fprintf(stderr, "failed to start server\n");
        return -1;
    }
}
