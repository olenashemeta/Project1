#ifndef LOGIN_H
#define LOGIN_H

#include "../inc/libs.h"

static void on_login_button_clicked(GtkButton *button, gpointer user_data);
void activate_login_window(GtkApplication *app);
static void activate(GtkApplication *app, gpointer user_data);

#endif