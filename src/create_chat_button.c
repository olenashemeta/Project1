#include "../inc/client.h"

// void on_create_chat_clicked(GtkButton *button, gpointer user_data) {
//     (void)button;
//     (void)user_data;
//     GtkWidget *dialog = gtk_dialog_new_with_buttons(
//         "Create new chat",
//         NULL,
//         GTK_DIALOG_MODAL,
//         "Private chat", GTK_RESPONSE_ACCEPT,
//         "Group chat", GTK_RESPONSE_REJECT,
//         "Cancel", GTK_RESPONSE_CANCEL,
//         NULL
//     );

//     gint response = gtk_dialog_run(GTK_DIALOG(dialog));
//     if (response == GTK_RESPONSE_ACCEPT) {
//         add_chat_item(NULL, "Новий приватний чат");
//     } else if (response == GTK_RESPONSE_REJECT) {
//         add_chat_item(NULL, "Нова група");
//     }

//     gtk_widget_destroy(dialog);
// }

void on_create_chat_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Create new chat",
        NULL,
        GTK_DIALOG_MODAL,
        "Private chat", GTK_RESPONSE_ACCEPT,
        "Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (response == GTK_RESPONSE_ACCEPT) {
        GtkWidget *name_dialog = gtk_dialog_new_with_buttons(
            "Search user",
            NULL,
            GTK_DIALOG_MODAL,
            "OK", GTK_RESPONSE_ACCEPT,
            "Cancel", GTK_RESPONSE_CANCEL,
            NULL
        );
        GtkWidget *user_search_entry = gtk_entry_new();
    
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
        gtk_box_pack_start(GTK_BOX(box), user_search_entry, TRUE, TRUE, 0);

        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(name_dialog));
        gtk_container_add(GTK_CONTAINER(content_area), box);

        gtk_widget_show_all(name_dialog);

        gint name_response = gtk_dialog_run(GTK_DIALOG(name_dialog));

        const char *search_query = NULL;

        if (name_response == GTK_RESPONSE_ACCEPT) {

            search_query = gtk_entry_get_text(GTK_ENTRY(user_search_entry));

            add_chat_item(NULL, search_query);

            printf("Search query: %s\n", search_query);

            t_group* new_group = create_group(search_query, 1, write_username());
            printf("Name of group: %s\t Is_private: %i\t Who created: %s\n", new_group->name, new_group->is_private, new_group->creator_username);

            // відправлення на сервер та перевірка чи є такий юзер

            free_group(new_group);
        }

        gtk_widget_destroy(name_dialog);
    }
}








