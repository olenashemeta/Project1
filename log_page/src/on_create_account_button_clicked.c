#include "log_page.h"

void on_create_account_button_clicked(GtkWidget *button, gpointer data) {
    GtkWidget *vbox = GTK_WIDGET(data);

    GList *children = gtk_container_get_children(GTK_CONTAINER(vbox));
    for (GList *l = children; l != NULL; l = l->next) {
        gtk_widget_destroy(GTK_WIDGET(l->data));
    }

    GtkWidget *create_account_label = gtk_label_new("Create Account");

    GtkWidget *new_username_label = gtk_label_new("Username:");
    GtkWidget *new_username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(new_username_entry), "Enter username");

    GtkWidget *login_label = gtk_label_new("Login:");
    GtkWidget *login_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(login_entry), "Enter login");

    GtkWidget *new_password_label = gtk_label_new("Password:");
    GtkWidget *new_password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(new_password_entry), "Enter password");  
    gtk_entry_set_visibility(GTK_ENTRY(new_password_entry), FALSE);

    GtkWidget *confirm_password_label = gtk_label_new("Confirm Password:");
    GtkWidget *confirm_password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_password_entry), "Enter password again"); 
    gtk_entry_set_visibility(GTK_ENTRY(confirm_password_entry), FALSE); 

    GtkWidget *error_label = gtk_label_new("");
    gtk_widget_set_name(error_label, "error-label");

    GtkWidget *submit_create_account_button = gtk_button_new_with_label("Submit Account");
    GtkWidget *back_to_login_button = gtk_button_new_with_label("Back to Login");

    gtk_box_pack_start(GTK_BOX(vbox), create_account_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), new_username_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), new_username_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), login_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), login_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), new_password_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), new_password_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), confirm_password_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), confirm_password_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), error_label, FALSE, FALSE, 5); 
    gtk_box_pack_start(GTK_BOX(vbox), submit_create_account_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), back_to_login_button, FALSE, FALSE, 5);

    g_object_set_data(G_OBJECT(submit_create_account_button), "new_username_entry", new_username_entry);
    g_object_set_data(G_OBJECT(submit_create_account_button), "login_entry", login_entry);
    g_object_set_data(G_OBJECT(submit_create_account_button), "new_password_entry", new_password_entry);
    g_object_set_data(G_OBJECT(submit_create_account_button), "confirm_password_entry", confirm_password_entry);
    g_object_set_data(G_OBJECT(submit_create_account_button), "error_label", error_label);

    g_signal_connect(submit_create_account_button, "clicked", G_CALLBACK(on_submit_account_button_clicked), vbox);

    g_signal_connect(back_to_login_button, "clicked", G_CALLBACK(restore_login_form), vbox);

    gtk_widget_show_all(vbox);
}

