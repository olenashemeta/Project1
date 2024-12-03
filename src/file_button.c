#include "../inc/client.h"

void on_file_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GtkWidget *parent_window = GTK_WIDGET(user_data);

    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Виберіть файл",
        GTK_WINDOW(parent_window),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Відмінити", GTK_RESPONSE_CANCEL,
        "_Відкрити", GTK_RESPONSE_ACCEPT,
        NULL
    );

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        g_print("Вибрано файл: %s\n", filename);

        add_message_to_chat(NULL, filename); 
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}
