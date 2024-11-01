#include "../inc/client.h"

static void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    GtkLabel *label = GTK_LABEL(user_data);
    gtk_label_set_text(label, "Reconnecting...");
    mx_connect_to_server(label);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *window_box;
    GtkWidget *button;
    GtkWidget *button_box;
    GtkWidget *status_label;

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

    g_signal_connect(button, "clicked", G_CALLBACK(on_login_button_clicked), status_label);
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("app.chat", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);

    g_object_unref(app);
    return status;
}

