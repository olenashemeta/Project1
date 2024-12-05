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
#include <time.h>
#include <glib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <librsvg/rsvg.h>

void mx_connect_to_server(GtkLabel *label);
void css_theme_to_window(GtkWidget *window, const char* theme);
void css_theme(const char* theme);
void show_chat_window(GtkApplication *app);

extern void on_chat_closed(GtkButton *button, gpointer user_data);
extern void clear_message_area();
extern void on_chat_selected(GtkListBox *box, GtkListBoxRow *row, gpointer user_data);
extern void add_chat_item(GtkButton *button, const char *chat_name);

extern void add_message_to_chat(const char *message, const char *file_path);
extern void add_message(GtkButton *button, gpointer user_data);

extern void on_create_chat_clicked(GtkButton *button, gpointer user_data);
extern void on_file_button_clicked(GtkButton *button, gpointer user_data);
extern void on_search_changed(GtkEntry *entry, gpointer user_data);

extern GtkWidget *chat_list;
extern GtkWidget *message_area;
extern GtkWidget *current_chat;

#endif
