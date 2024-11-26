#include "log_page.h"

void on_login_button_clicked(GtkWidget *button, gpointer data) {
    GtkWidget *username_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "username_entry"));
    GtkWidget *password_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "password_entry"));
    GtkWidget *error_label = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "error_label")); 

    const char *login = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    if (mx_strcmp(password, "") == 0 || mx_strcmp(login, "") == 0) {
        gtk_label_set_text(GTK_LABEL(error_label), "Fill in all fields");
        return;
    }
    
    t_user* user = mx_create_log_user(login, password);

    g_print("Login: %s\n", user->login);
    g_print("Password: %s\n", user->password);
    
    // wait for server

    mx_free_user(user);
}

