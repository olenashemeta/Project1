#ifndef WINDOW_H
#define WINDOW_H

#include "../inc/libs.h"
#include "../inc/client.h"

void css_theme(const char* theme);
static void activate(GtkApplication *app, gpointer user_data);
static void on_close_button_clicked(GtkWidget *widget, gpointer data);
static void on_maximize_button_clicked(GtkWidget *widget, gpointer data);
static void on_minimize_button_clicked(GtkWidget *widget, gpointer data);

#endif
