#include "../inc/bee_user.h"

void on_button_create_chat_cliked(void) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Create chat",
        NULL,
        GTK_DIALOG_MODAL,
        "Private chat", GTK_RESPONSE_ACCEPT,
        NULL
    );
    gtk_window_set_default_size(GTK_WINDOW(dialog), 100, 30);
    gtk_widget_set_name(dialog, "create-chat-dialog");

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (response == GTK_RESPONSE_ACCEPT) {
        GtkWidget *name_dialog = gtk_dialog_new_with_buttons(
            "Search user",
            NULL,
            GTK_DIALOG_MODAL,
            "OK", GTK_RESPONSE_ACCEPT,
            NULL
        );
        gtk_widget_set_name(name_dialog, "search-user-dialog");

        GtkWidget *user_search_entry = gtk_entry_new();
        gtk_widget_set_name(user_search_entry, "user-search-entry");

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
        gtk_widget_set_name(box, "search-entry-box");

        gtk_box_pack_start(GTK_BOX(box), user_search_entry, TRUE, TRUE, 0);


        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(name_dialog));
        gtk_container_add(GTK_CONTAINER(content_area), box);

        gtk_widget_show_all(name_dialog);

        gint name_response = gtk_dialog_run(GTK_DIALOG(name_dialog));

        const char *search_query = NULL;

        if (name_response == GTK_RESPONSE_ACCEPT) {
            search_query = gtk_entry_get_text(GTK_ENTRY(user_search_entry));

            if (search_query == NULL || strlen(search_query) == 0) {
                mx_printerr("Nothing print!!!");
            } else {
                mx_printstr("Search person:  ");
                mx_printstr(search_query);
                mx_printstr("\n");

                // Perform user search
            }
        }

        gtk_widget_destroy(name_dialog);
    }
}

