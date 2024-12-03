#include "../inc/client.h"

void add_message(GtkButton *button, gpointer user_data) {
    (void)button;
    GtkWidget *message_entry = GTK_WIDGET(user_data);
    const char *message = gtk_entry_get_text(GTK_ENTRY(message_entry));

    if (strlen(message) == 0 && current_chat == NULL) {
        return; 
    }

    add_message_to_chat(message, NULL);
    gtk_entry_set_text(GTK_ENTRY(message_entry), ""); 
}
