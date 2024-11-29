#include "../inc/log_page.h"

void create_log_page(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "BEE CHAT");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 700);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_set_name(window, "main-window");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    load_css(window);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *title_label = gtk_label_new("Log In");
    gtk_widget_set_name(title_label, "title-label");
    gtk_label_set_xalign(GTK_LABEL(title_label), 0.5);
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 5);

    GtkWidget *username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Enter your login");
    gtk_widget_set_name(username_entry, "username_entry"); 
    gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 5);

    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Enter your password");
    gtk_widget_set_name(password_entry, "password_entry");  
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); 
    gtk_entry_set_invisible_char(GTK_ENTRY(password_entry), 8226);
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 5);

    GtkWidget *error_label = gtk_label_new("");
    gtk_widget_set_name(error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(vbox), error_label, FALSE, FALSE, 5);

    GtkWidget *login_button = gtk_button_new_with_label("Enter hive");
    gtk_widget_set_name(login_button, "login_button");
    gtk_box_pack_start(GTK_BOX(vbox), login_button, FALSE, FALSE, 5);

    g_object_set_data(G_OBJECT(login_button), "username_entry", username_entry);
    g_object_set_data(G_OBJECT(login_button), "password_entry", password_entry);
    g_object_set_data(G_OBJECT(login_button), "error_label", error_label);

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);

    GtkWidget *create_account_button = gtk_button_new_with_label("Beecome membeer");
    gtk_widget_set_name(create_account_button, "create_account_button"); 
    gtk_box_pack_start(GTK_BOX(vbox), create_account_button, FALSE, FALSE, 5);

    g_signal_connect(create_account_button, "clicked", G_CALLBACK(on_create_account_button_clicked), vbox);

    GtkWidget *center_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(center_box), vbox);

    gtk_container_add(GTK_CONTAINER(window), center_box);

    gtk_widget_set_halign(center_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(center_box, GTK_ALIGN_CENTER);

    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();
}



int main(int argc, char *argv[]) {
    create_log_page(argc, argv);
    return 0;
}



