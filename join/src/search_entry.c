#include "../inc/client.h"

void on_search_changed(GtkEntry *entry, gpointer user_data) {
    (void)user_data;
    const char *search_text = gtk_entry_get_text(entry);
    GList *rows = gtk_container_get_children(GTK_CONTAINER(chat_list));

    for (GList *iter = rows; iter != NULL; iter = iter->next) {
        GtkWidget *row = GTK_WIDGET(iter->data);
        if (!GTK_IS_WIDGET(row)) {
            continue; 
        }

        GtkWidget *box = gtk_bin_get_child(GTK_BIN(row)); 
        if (!GTK_IS_CONTAINER(box)) {
            continue; 
        }

        GList *box_children = gtk_container_get_children(GTK_CONTAINER(box));
        GtkWidget *label = GTK_WIDGET(g_list_nth_data(box_children, 0));

        if (GTK_IS_LABEL(label)) {
            const char *chat_name = gtk_label_get_text(GTK_LABEL(label));
            gboolean visible = g_strrstr(chat_name, search_text) != NULL;
            gtk_widget_set_visible(row, visible);
        }

        g_list_free(box_children); 
    }
    g_list_free(rows); 
}
