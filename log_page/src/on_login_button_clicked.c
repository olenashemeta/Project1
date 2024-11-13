#include "log_page.h"

void on_login_button_clicked(GtkWidget *button, gpointer data) {
    const char *login = gtk_entry_get_text(GTK_ENTRY(data));
    const char *password = gtk_entry_get_text(GTK_ENTRY(g_object_get_data(G_OBJECT(button), "password_entry"))); 
    GtkWidget *error_label = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "error_label")); 

    if ( mx_strcmp(password, "") == 0 || mx_strcmp(login, "") == 0) {
        gtk_label_set_text(GTK_LABEL(error_label), "Fill in all fields");
        return;
    }
     
	t_user* user = mx_create_log_user(login, password);
	
    g_print("Login: %s\n", user->login);
    g_print("Password: %s\n", user->password);
	
	// очікування відповіді від сервера(створення нового вікна)
	mx_free_user(user);
}
