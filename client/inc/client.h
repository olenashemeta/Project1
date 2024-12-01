#ifndef CLIENT_H
#define CLIENT_H

//#include "../lib/libmx/inc/libmx.h"
#include "../inc/libs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <gtk/gtk.h>

static void on_login_button_clicked(GtkButton *button, gpointer user_data);
static void activate(GtkApplication *app, gpointer user_data);
void css_theme(const char* theme);
void mx_connect_to_server(GtkLabel *label);
void show_chat_window(GtkApplication *app);
void on_chat_button_clicked(GtkWidget *widget, gpointer data);

#endif