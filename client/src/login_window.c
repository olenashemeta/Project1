#include "../inc/client.h"

static void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    GtkLabel *label = GTK_LABEL(user_data);

    t_main *main = (t_main *)g_object_get_data(G_OBJECT(button), "main_data");
    if (!main || main->socket < 0) {
        gtk_label_set_text(label, "No connection to server");
        return;
    }

    t_user *user = mx_create_client();
    printf("Created test client:\n");
    mx_print_client(user);
    

    cJSON *login_request = form_login_request(user->login, user->password);

    prepare_and_send_json(login_request, main);

    mx_free_client(user);
}
 
void login_window(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *window_box;
    GtkWidget *button;
    GtkWidget *button_box;
    GtkWidget *status_label;
    t_main *main = (t_main *)user_data;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "CHAT");
    gtk_window_set_default_size(GTK_WINDOW(window), 480, 900);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    
    window_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(window), window_box);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(window_box), button_box, TRUE, TRUE, 0);

    button = gtk_button_new_with_label("Log in");
    gtk_container_add(GTK_CONTAINER(button_box), button);

    status_label = gtk_label_new("Status: Disconnected");
    gtk_box_pack_start(GTK_BOX(window_box), status_label, TRUE, TRUE, 0);

    g_object_set_data(G_OBJECT(button), "main_data", main);

    g_signal_connect(button, "clicked", G_CALLBACK(on_login_button_clicked), status_label);
    gtk_widget_show_all(window);
}
