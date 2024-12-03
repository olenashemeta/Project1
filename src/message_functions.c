#include "../inc/client.h"
#include <time.h>

char* get_current_time() {
    time_t rawtime;
    struct tm *timeinfo;
    static char time_buffer[6]; 

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_buffer, sizeof(time_buffer), "%H:%M", timeinfo);
    return time_buffer;
}

void add_message_to_chat(const char *message, const char *file_path) {
    if (current_chat == NULL) {
        g_print("Немає вибраного чату для додавання повідомлення.\n");
        return;
    }

    GtkWidget *message_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_margin_top(message_box, 5);
    gtk_widget_set_margin_bottom(message_box, 5);

    gboolean is_user_message = (message != NULL); 

    gtk_widget_set_halign(message_box, GTK_ALIGN_END);

    GtkWidget *content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);

    if (message && strlen(message) > 0) {
        GtkWidget *label = gtk_label_new(message);
        gtk_label_set_xalign(GTK_LABEL(label), 0.0); 
        gtk_widget_set_name(label, "message-label"); 
        gtk_box_pack_start(GTK_BOX(content_box), label, FALSE, FALSE, 2);
    }

    if (file_path && strlen(file_path) > 0) {
        GError *error = NULL;
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale(file_path, 200, 200, TRUE, &error);
        if (pixbuf) {
            GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
            gtk_box_pack_start(GTK_BOX(content_box), image, FALSE, FALSE, 2);
            g_object_unref(pixbuf);
        } else {
            g_print("Помилка завантаження зображення: %s\n", error->message);
            g_error_free(error);
        }
    }

    GtkWidget *time_label = gtk_label_new(get_current_time());
    gtk_widget_set_name(time_label, "time-label");
    gtk_label_set_xalign(GTK_LABEL(time_label), 1.0);
    gtk_box_pack_start(GTK_BOX(content_box), time_label, FALSE, FALSE, 2);

    gtk_box_pack_start(GTK_BOX(message_box), content_box, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(message_area), message_box, FALSE, FALSE, 5);

    gtk_widget_show_all(message_box);
}


