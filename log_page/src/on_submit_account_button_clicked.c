#include "../inc/log_page.h"

void on_submit_account_button_clicked(GtkWidget *button, gpointer data) {
    GtkWidget *vbox = GTK_WIDGET(data);
    (void)vbox;

    GtkWidget *username_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "username_entry"));
    GtkWidget *login_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "login_entry"));
    GtkWidget *password_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "password_entry"));
    GtkWidget *confirm_password_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "confirm_password_entry"));
    GtkWidget *error_label = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "error_label"));
    gtk_widget_set_name(error_label, "error-label");
    
    const char *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const char *login = gtk_entry_get_text(GTK_ENTRY(login_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(password_entry));
    const char *confirm_password = gtk_entry_get_text(GTK_ENTRY(confirm_password_entry));
    //fill all gap
    if (mx_strcmp(username, "") == 0 || mx_strcmp(password, "") == 0 || mx_strcmp(login, "") == 0) {
        gtk_label_set_text(GTK_LABEL(error_label), "Fill in all fields");
        return;
    }
    //log 1000
    if (mx_strlen(login) > MAX_SIZE) {
        gtk_label_set_text(GTK_LABEL(error_label), "Login must be\nless than 1000 characters");
        return;
    } else {
        gtk_label_set_text(GTK_LABEL(error_label), ""); 
    }
    // pas 8
    if (mx_strlen(password) < 8) {
        gtk_label_set_text(GTK_LABEL(error_label), "Password must be\nlonger than 8 characters");
        return;
    } else {
        gtk_label_set_text(GTK_LABEL(error_label), ""); 
    }
    // pas 1000
    if (mx_strlen(password) > MAX_SIZE) {
        gtk_label_set_text(GTK_LABEL(error_label), "Password must be\nless than 1000 characters");
        return;
    } else {
        gtk_label_set_text(GTK_LABEL(error_label), ""); 
    }

    if (mx_strcmp(password, confirm_password) != 0) {
        gtk_label_set_text(GTK_LABEL(error_label), "Passwords don't match!");
        return;
    } else {
        gtk_label_set_text(GTK_LABEL(error_label), ""); 
    }

   	t_user* user = mx_create_user(login, username, password);
	
    g_print("Account created successfully:\n");
    g_print("Username: %s\n", user->name);
    g_print("Login: %s\n", user->login);
    g_print("Password: %s\n", user->password);
	// відповідь від сервера
	
	mx_free_user(user);
	
}
