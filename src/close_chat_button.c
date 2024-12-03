#include "../inc/client.h"

void on_chat_closed(GtkButton *button, gpointer user_data) {
    GtkWidget *chat_item = GTK_WIDGET(user_data);
    gtk_widget_destroy(chat_item);
    if (chat_item == current_chat) {
        current_chat = NULL;
        clear_message_area(); 
    }
}
