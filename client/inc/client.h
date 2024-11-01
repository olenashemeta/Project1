#ifndef CLIENT_H
#define CLIENT_H

//#include "../lib/libmx/inc/libmx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <gtk/gtk.h>

void mx_connect_to_server(GtkLabel *label);

#endif