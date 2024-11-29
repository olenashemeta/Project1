#include "log_page.h"

void restore_login_form(GtkWidget *button, gpointer data) {
    GtkWidget *vbox = GTK_WIDGET(data);

    GList *children = gtk_container_get_children(GTK_CONTAINER(vbox));
    for (GList *l = children; l != NULL; l = l->next) {
        gtk_widget_destroy(GTK_WIDGET(l->data));
    }

    GtkWidget *title_label = gtk_label_new("Log In");
    gtk_widget_set_name(title_label, "title-label");
    gtk_label_set_xalign(GTK_LABEL(title_label), 0.5);
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 5);

    GtkWidget *username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Enter your login");
    gtk_widget_set_name(username_entry, "entry"); 

    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Enter your password");
    gtk_widget_set_name(password_entry, "entry"); 
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    GtkWidget *error_label = gtk_label_new("");
    gtk_widget_set_name(error_label, "error-label");

    gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), error_label, FALSE, FALSE, 5);

    GtkWidget *login_button = gtk_button_new_with_label("Enter hire");
    gtk_box_pack_start(GTK_BOX(vbox), login_button, FALSE, FALSE, 5);

    g_object_set_data(G_OBJECT(login_button), "username_entry", username_entry);
    g_object_set_data(G_OBJECT(login_button), "password_entry", password_entry);
    g_object_set_data(G_OBJECT(login_button), "error_label", error_label);

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);

    GtkWidget *create_account_button = gtk_button_new_with_label("Become *a* membeer");
    gtk_widget_set_name(create_account_button, "create-account");
    gtk_box_pack_start(GTK_BOX(vbox), create_account_button, FALSE, FALSE, 5);

    g_signal_connect(create_account_button, "clicked", G_CALLBACK(on_create_account_button_clicked), vbox);

    gtk_widget_show_all(vbox);
}




