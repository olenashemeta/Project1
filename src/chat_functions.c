#include "../inc/client.h"

void add_chat_item(GtkButton *button, const char *chat_name) {
    GtkWidget *chat_item = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *label = gtk_label_new(chat_name);
    GtkWidget *close_button = gtk_button_new_with_label("X");

    gtk_box_pack_start(GTK_BOX(chat_item), label, TRUE, TRUE, 5);
    gtk_box_pack_end(GTK_BOX(chat_item), close_button, FALSE, FALSE, 5);

    g_signal_connect(close_button, "clicked", G_CALLBACK(on_chat_closed), chat_item);

    GtkWidget *row = gtk_list_box_row_new();
    gtk_container_add(GTK_CONTAINER(row), chat_item);
    gtk_list_box_insert(GTK_LIST_BOX(chat_list), row, -1);

    gtk_widget_show_all(row);
}

void clear_message_area() {
    GList *children = gtk_container_get_children(GTK_CONTAINER(message_area));
    for (GList *iter = children; iter != NULL; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
}

void on_chat_selected(GtkListBox *box, GtkListBoxRow *row, gpointer user_data) {
    if (GTK_WIDGET(row) != current_chat) {
        current_chat = GTK_WIDGET(row);
        clear_message_area(); 
    }
}
