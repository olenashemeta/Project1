#include "../inc/client.h"

void add_message(GtkButton *button, gpointer user_data) {
    (void)button;
    g_print("Button handler started\n");

    GtkWidget *message_entry = GTK_WIDGET(user_data);
    const char *message = gtk_entry_get_text(GTK_ENTRY(message_entry));

    g_print("Message: %s\n", message);

    if (strlen(message) == 0 || current_chat == NULL) {
        g_print("No message or current chat\n");
        return;
    }

    g_print("Message sent\n");
    add_message_to_chat(message, NULL);
    gtk_entry_set_text(GTK_ENTRY(message_entry), ""); 
}
