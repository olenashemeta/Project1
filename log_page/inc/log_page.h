#ifndef LOG_PAGE_H
#define LOG_PAGE_H

#include <gtk/gtk.h>
#include "../libmx/inc/libmx.h"

typedef struct s_user {
    char* login;
    char* name;
    char* password;
} t_user;


void on_create_account_button_clicked(GtkWidget *button, gpointer data);
void on_submit_account_button_clicked(GtkWidget *button, gpointer data);
void on_login_button_clicked(GtkWidget *button, gpointer data);
void restore_login_form(GtkWidget *button, gpointer data);
void apply_css(GtkWidget *window);
t_user* mx_create_log_user(const char* login, const char* password);
t_user* mx_create_user(const char* login, const char* name, const char* password);
void mx_free_user(t_user* new_user);	


#endif
