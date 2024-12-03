#include "../inc/client.h"

void on_create_chat_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Створити чат",
        NULL,
        GTK_DIALOG_MODAL,
        "Приватний чат", GTK_RESPONSE_ACCEPT,
        "Груповий чат", GTK_RESPONSE_REJECT,
        "Відмінити", GTK_RESPONSE_CANCEL,
        NULL
    );

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        add_chat_item(NULL, "Новий приватний чат");
    } else if (response == GTK_RESPONSE_REJECT) {
        add_chat_item(NULL, "Нова група");
    }

    gtk_widget_destroy(dialog);
}
