#include "log_page.h"

void create_log_page(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "BEE CHAT");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 700);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);  

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *username_label = gtk_label_new("Login:");
    GtkWidget *username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Enter your login"); 
    gtk_box_pack_start(GTK_BOX(vbox), username_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 5);

    GtkWidget *password_label = gtk_label_new("Password:");
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Enter your password"); 
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); 
    gtk_entry_set_invisible_char(GTK_ENTRY(password_entry), 8226);
    gtk_box_pack_start(GTK_BOX(vbox), password_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 5);

    GtkWidget *error_label = gtk_label_new("");
    gtk_widget_set_name(error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(vbox), error_label, FALSE, FALSE, 5);

    // Login button
    GtkWidget *login_button = gtk_button_new_with_label("Login");
    gtk_box_pack_start(GTK_BOX(vbox), login_button, FALSE, FALSE, 5);

    g_object_set_data(G_OBJECT(login_button), "password_entry", password_entry);
    g_object_set_data(G_OBJECT(login_button), "error_label", error_label);

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), username_entry);

    GtkWidget *create_account_button = gtk_button_new_with_label("Create Account");
    gtk_widget_set_name(create_account_button, "create-account"); 
    gtk_box_pack_start(GTK_BOX(vbox), create_account_button, FALSE, FALSE, 5);

    g_signal_connect(create_account_button, "clicked", G_CALLBACK(on_create_account_button_clicked), vbox);

    GtkWidget *center_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(center_box), vbox);

    gtk_container_add(GTK_CONTAINER(window), center_box);

    gtk_widget_set_halign(center_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(center_box, GTK_ALIGN_CENTER);

    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    // Apply CSS styles
    apply_css(window);

    gtk_widget_show_all(window);

    gtk_main();
}


int main(int argc, char *argv[]) {
    create_log_page(argc, argv);
    return 0;
}
