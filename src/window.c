#include "../inc/client.h"
#include <time.h>

void add_message_to_chat(const char *message, const char *file_path);

GtkWidget *chat_list;    
GtkWidget *message_area; 
GtkWidget *current_chat; 

void on_chat_closed(GtkButton *button, gpointer user_data);
void add_chat_item(GtkButton *button, const char *chat_name);

void show_chat_window(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Чат");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window), paned);

    GtkWidget *left_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *search_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_entry), "Пошук чату...");
    gtk_widget_set_name(search_entry, "search-entry");
    gtk_box_pack_start(GTK_BOX(left_panel), search_entry, FALSE, FALSE, 5);
    
    g_signal_connect(search_entry, "changed", G_CALLBACK(on_search_changed), NULL);

    GtkWidget *scrollable_chat_list = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_chat_list),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_name(scrollable_chat_list, "chat-list-scroll");

    chat_list = gtk_list_box_new();
    gtk_widget_set_name(chat_list, "chat-list-box");
    g_signal_connect(chat_list, "row-selected", G_CALLBACK(on_chat_selected), NULL);

    gtk_container_add(GTK_CONTAINER(scrollable_chat_list), chat_list);
    gtk_box_pack_start(GTK_BOX(left_panel), scrollable_chat_list, TRUE, TRUE, 5);

    GtkWidget *create_chat_button = gtk_button_new_with_label("+ Створити чат");
    gtk_widget_set_name(create_chat_button, "create-chat-button");
    g_signal_connect(create_chat_button, "clicked", G_CALLBACK(on_create_chat_clicked), NULL);
    gtk_box_pack_end(GTK_BOX(left_panel), create_chat_button, FALSE, FALSE, 5);

    gtk_paned_add1(GTK_PANED(paned), left_panel);

    GtkWidget *right_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *scrollable_message_area = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_message_area),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_name(scrollable_message_area, "message-area-scroll");

    message_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(scrollable_message_area), message_area);

    gtk_box_pack_start(GTK_BOX(right_panel), scrollable_message_area, TRUE, TRUE, 5);

    GtkWidget *entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    GtkWidget *file_button = gtk_button_new_with_label("📁");
    gtk_box_pack_start(GTK_BOX(entry_box), file_button, FALSE, FALSE, 5);
    g_signal_connect(file_button, "clicked", G_CALLBACK(on_file_button_clicked), window);

    GtkWidget *message_entry = gtk_entry_new();
    GtkWidget *send_button = gtk_button_new_with_label("Відправити");

    gtk_box_pack_start(GTK_BOX(entry_box), message_entry, TRUE, TRUE, 5);
    gtk_box_pack_end(GTK_BOX(entry_box), send_button, FALSE, FALSE, 5);

    gtk_box_pack_end(GTK_BOX(right_panel), entry_box, FALSE, FALSE, 5);

    g_signal_connect(send_button, "clicked", G_CALLBACK(add_message), message_entry);

    gtk_paned_add2(GTK_PANED(paned), right_panel);
    
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "styles.css", NULL);
    GtkStyleContext *style_context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    gtk_widget_show_all(window);
}

