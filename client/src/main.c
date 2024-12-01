#include "../inc/client.h"

void activate_login_window(GtkApplication *app);

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("app.chat", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate_login_window), NULL);
    css_theme("css/theme.css");
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
