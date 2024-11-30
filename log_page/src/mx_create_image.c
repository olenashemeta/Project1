#include "../inc/log_page.h"

GtkWidget* create_image(const char *image_path, int width, int height) {
    GdkPixbuf *load_img = gdk_pixbuf_new_from_file(image_path, NULL);
    if (!load_img) {
        g_warning("Can't load image: %s", image_path);
        return NULL;
    }

    GdkPixbuf *format_img = gdk_pixbuf_scale_simple(load_img, width, height, GDK_INTERP_BILINEAR);
    
    GtkWidget *image = gtk_image_new_from_pixbuf(format_img);

    g_object_unref(load_img);
    g_object_unref(format_img);

    return image;
}