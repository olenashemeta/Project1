#include "../inc/log_page.h"

void on_create_account_button_clicked(GtkWidget *button, gpointer data) {
    GtkWidget *vbox = GTK_WIDGET(data);

    GList *children = gtk_container_get_children(GTK_CONTAINER(vbox));
    for (GList *l = children; l != NULL; l = l->next) {
        gtk_widget_destroy(GTK_WIDGET(l->data));
    }

    GtkWidget *create_account_label = gtk_label_new("Create Account");
    gtk_widget_set_name(create_account_label, "title-label");
    GtkWidget *username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Enter username");
    gtk_widget_set_name(username_entry, "username_entry"); 

    GtkWidget *login_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(login_entry), "Enter login");
    gtk_widget_set_name(login_entry, "login_entry"); 

    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Enter password"); 
    gtk_widget_set_name(password_entry, "password_entry");  
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    GtkWidget *confirm_password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_password_entry), "Enter password again");
     gtk_widget_set_name(confirm_password_entry, "confirm_password_entry");
    gtk_entry_set_visibility(GTK_ENTRY(confirm_password_entry), FALSE); 

    GtkWidget *error_label = gtk_label_new("");
    gtk_widget_set_name(error_label, "error_label");

    GtkWidget *submit_create_account_button = gtk_button_new_with_label("Submit Account");
    gtk_widget_set_name(submit_create_account_button, "submit_create_account_button");
    GtkWidget *back_to_login_button = gtk_button_new_with_label("Back to Log In");
    gtk_widget_set_name(back_to_login_button, "back_to_login_button");

    gtk_box_pack_start(GTK_BOX(vbox), create_account_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), login_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), confirm_password_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), error_label, FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(vbox), submit_create_account_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), back_to_login_button, FALSE, FALSE, 5);

    g_object_set_data(G_OBJECT(submit_create_account_button), "username_entry", username_entry);
    g_object_set_data(G_OBJECT(submit_create_account_button), "login_entry", login_entry);
    g_object_set_data(G_OBJECT(submit_create_account_button), "password_entry", password_entry);
    g_object_set_data(G_OBJECT(submit_create_account_button), "confirm_password_entry", confirm_password_entry);
    g_object_set_data(G_OBJECT(submit_create_account_button), "error_label", error_label);

    g_signal_connect(submit_create_account_button, "clicked", G_CALLBACK(on_submit_account_button_clicked), vbox);

    g_signal_connect(back_to_login_button, "clicked", G_CALLBACK(restore_login_form), vbox);

    gtk_widget_show_all(vbox);
}

