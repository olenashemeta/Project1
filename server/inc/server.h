#ifndef SERVER_H
#define SERVER_H

//#include "../lib/libmx/inc/libmx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>


typedef struct s_client
{
    int client_id;
    int socket_fd;
    struct sockaddr_in address;
}              t_client;

void mx_daemon_start(void);
void mx_daemon_end(int signal);
void set_signal(void);

#endif 