#ifndef WINDOW_H
#define WINDOW_H

#include "../inc/libs.h"
#include "../inc/client.h"
#include <librsvg/rsvg.h>

void css_theme(const char* theme);
static void activate(GtkApplication *app, gpointer user_data);
extern GtkImage *avatar_image_global;
void set_window_background_from_svg(GtkWidget *overlay, const char *svg_path, int width, int height);
void show_change_avatar_menu(GtkWidget *parent);
void show_change_password_menu(GtkWidget *parent);
void show_change_username_menu(GtkWidget *parent);
void show_logout_menu(GtkWidget *parent);
void on_change_username(GtkButton *button, gpointer user_data);
void on_change_password(GtkButton *button, gpointer user_data);
void on_logout(GtkButton *button, gpointer user_data);
void on_settings_clicked(GtkButton *button, gpointer user_data);
void show_chat_window(GtkApplication *app);

void css_theme(const char* theme);
static void activate(GtkApplication *app, gpointer user_data);
static void on_close_button_clicked(GtkWidget *widget, gpointer data);
static void on_maximize_button_clicked(GtkWidget *widget, gpointer data);
static void on_minimize_button_clicked(GtkWidget *widget, gpointer data);

#endif
