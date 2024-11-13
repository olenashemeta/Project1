#include "log_page.h"

void restore_login_form(GtkWidget *button, gpointer data) {
    GtkWidget *vbox = GTK_WIDGET(data);

    GList *children = gtk_container_get_children(GTK_CONTAINER(vbox));
    for (GList *l = children; l != NULL; l = l->next) {
        gtk_widget_destroy(GTK_WIDGET(l->data));
    }

    GtkWidget *username_label = gtk_label_new("Login:");
    GtkWidget *username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Enter your login"); 
    GtkWidget *password_label = gtk_label_new("Password:");
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Enter your password");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); 
    GtkWidget *error_label = gtk_label_new("");
    gtk_widget_set_name(error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(vbox), username_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), password_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), error_label, FALSE, FALSE, 5);
    
    GtkWidget *login_button = gtk_button_new_with_label("Login");
    gtk_box_pack_start(GTK_BOX(vbox), login_button, FALSE, FALSE, 5);

    g_object_set_data(G_OBJECT(login_button), "password_entry", password_entry);
    g_object_set_data(G_OBJECT(login_button), "error_label", error_label);

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), username_entry);

    GtkWidget *create_account_button = gtk_button_new_with_label("Create Account");
    gtk_widget_set_name(create_account_button, "create-account"); 
    gtk_box_pack_start(GTK_BOX(vbox), create_account_button, FALSE, FALSE, 5);

    g_signal_connect(create_account_button, "clicked", G_CALLBACK(on_create_account_button_clicked), vbox);

    gtk_widget_show_all(vbox);
}


