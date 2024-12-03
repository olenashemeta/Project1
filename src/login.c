#include "../inc/login.h"
#include "../inc/client.h"

static void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GtkLabel *label = GTK_LABEL(user_data);
    gtk_label_set_text(label, "Reconnecting...");
    mx_connect_to_server(label);
    GtkApplication *app = g_object_get_data(G_OBJECT(label), "app");
    show_chat_window(app);
}

void activate_login_window(GtkApplication *app) {
    GtkWidget *window;
    GtkWidget *window_box;
    GtkWidget *button;
    GtkWidget *button_box;
    GtkWidget *status_label;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "CHAT - Login");
    gtk_window_set_default_size(GTK_WINDOW(window), 480, 200);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    window_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), window_box);
    gtk_container_set_border_width(GTK_CONTAINER(window_box), 20);

    GtkWidget *username_label = gtk_label_new("Логин:");
    GtkWidget *username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(window_box), username_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(window_box), username_entry, FALSE, FALSE, 5);

    GtkWidget *password_label = gtk_label_new("Пароль:");
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(window_box), password_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(window_box), password_entry, FALSE, FALSE, 5);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 10);
    gtk_box_pack_start(GTK_BOX(window_box), button_box, FALSE, FALSE, 5);

    button = gtk_button_new_with_label("Войти");
    gtk_container_add(GTK_CONTAINER(button_box), button);

    status_label = gtk_label_new("Status: Disconnected");
    gtk_box_pack_start(GTK_BOX(window_box), status_label, FALSE, FALSE, 5);

    g_object_set_data(G_OBJECT(status_label), "app", app);

    g_signal_connect(button, "clicked", G_CALLBACK(on_login_button_clicked), status_label);
    gtk_widget_show_all(window);
}
