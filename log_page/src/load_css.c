#include "../inc/log_page.h"

void load_css(GtkWidget *window) {
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new();

    GError *error = NULL;
    if (!gtk_css_provider_load_from_path(provider, "styles.css", &error)) {
        g_warning("Failed to load CSS: %s", error->message);
        g_error_free(error);
    }

    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_style_context_add_provider(gtk_widget_get_style_context(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
}