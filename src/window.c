#include "../inc/client.h"
#include <time.h>


GtkImage *avatar_image_global = NULL;
GtkWidget *chat_list;    
GtkWidget *message_area; 
GtkWidget *current_chat;

char* username = "User Name";

void add_message_to_chat(const char *message, const char *file_path);
void on_chat_closed(GtkButton *button, gpointer user_data);
void add_chat_item(GtkButton *button, const char *chat_name);

char* write_username(){
    return username;
}

void set_window_background_from_svg(GtkWidget *overlay, const char *svg_path, int width, int height) {
    if (!g_file_test(svg_path, G_FILE_TEST_EXISTS)) {
        g_printerr("SVG file does not exist: %s\n", svg_path);
        return;
    }

    GError *error = NULL;
    RsvgHandle *svg_handle = rsvg_handle_new_from_file(svg_path, &error);
    if (!svg_handle) {
        g_printerr("Failed to load SVG file: %s\nError: %s\n", svg_path, error->message);
        g_error_free(error);
        return;
    }

    RsvgDimensionData dimensions;
    rsvg_handle_get_dimensions(svg_handle, &dimensions);

    double svg_width = dimensions.width;
    double svg_height = dimensions.height;

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);

    cairo_scale(cr, (double)width / svg_width, (double)height / svg_height);

    if (!rsvg_handle_render_cairo(svg_handle, cr)) {
        g_printerr("Failed to render SVG\n");
        cairo_destroy(cr);
        cairo_surface_destroy(surface);
        g_object_unref(svg_handle);
        return;
    }

    cairo_destroy(cr);
    g_object_unref(svg_handle);

    GtkWidget *background = gtk_image_new_from_surface(surface);
    cairo_surface_destroy(surface);

    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), background);
    gtk_overlay_set_overlay_pass_through(GTK_OVERLAY(overlay), background, TRUE);
    gtk_widget_show(background);
}

void on_avatar_selected(GtkButton *button, gpointer user_data) {
    (void)button;
    const char *avatar_path = (const char *)user_data;
    gtk_image_set_from_file(GTK_IMAGE(avatar_image_global), avatar_path);
    g_print("Avatar changed to: %s\n", avatar_path);
}

void show_change_avatar_menu(GtkWidget *parent) {
    if (!GTK_IS_WIDGET(parent)) {
        g_printerr("Invalid parent widget for popover\n");
        return;
    }

    const char *avatar_paths[] = {
        "img/bee1.png",
        "img/bee2.png",
        "img/bee3.png",
        "img/bee4.png"
    };
    const char *button_names[] = {
        "avatar-button-1",
        "avatar-button-2",
        "avatar-button-3",
        "avatar-button-4"
    };
    size_t avatar_count = sizeof(avatar_paths) / sizeof(avatar_paths[0]);

    GtkWidget *popover = gtk_popover_new(parent);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Создаём CSS-провайдер
    GtkCssProvider *css_provider = gtk_css_provider_new();
    char *current_dir = g_get_current_dir();
    char css_path[PATH_MAX];
    snprintf(css_path, sizeof(css_path), "%s/css/avatar.css", current_dir);
    gtk_css_provider_load_from_path(css_provider, css_path, NULL);
    g_free(current_dir);

    // Создаём кнопки аватаров
    for (size_t i = 0; i < avatar_count; i++) {
        GtkWidget *avatar_button = gtk_button_new();
        gtk_widget_set_name(avatar_button, button_names[i]);

        GtkStyleContext *context = gtk_widget_get_style_context(avatar_button);
        gtk_style_context_add_provider(context,
                                       GTK_STYLE_PROVIDER(css_provider),
                                       GTK_STYLE_PROVIDER_PRIORITY_USER);

        g_signal_connect(avatar_button, "clicked", G_CALLBACK(on_avatar_selected), (gpointer)avatar_paths[i]);
        gtk_box_pack_start(GTK_BOX(box), avatar_button, FALSE, FALSE, 5);
    }

    g_object_unref(css_provider);
    gtk_container_add(GTK_CONTAINER(popover), box);
    gtk_widget_show_all(popover);
}

void show_change_password_menu(GtkWidget *parent) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Password change is under development.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_change_username_menu(GtkWidget *parent) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Username change is under development.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_logout_menu(GtkWidget *parent) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_YES_NO,
                                               "Are you sure you want to log out?");
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_YES) {
        g_print("User chose to log out.\n");
    } else {
        g_print("User canceled logout.\n");
    }
    gtk_widget_destroy(dialog);
}

void on_settings_clicked(GtkButton *button, gpointer user_data) {
    (void)user_data;
    GtkWidget *popover = gtk_popover_new(GTK_WIDGET(button));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *change_avatar_button = gtk_button_new_with_label("Change Avatar");
    gtk_widget_set_name(change_avatar_button, "change_avatar_button");

    GtkWidget *change_password_button = gtk_button_new_with_label("Change Password");
    gtk_widget_set_name(change_password_button, "change_password_button");

    GtkWidget *change_username_button = gtk_button_new_with_label("Change Username");
    gtk_widget_set_name(change_username_button, "change_username_button");

    GtkWidget *logout_button = gtk_button_new_with_label("Logout");
    gtk_widget_set_name(logout_button, "logout_button");

    g_signal_connect(change_avatar_button, "clicked", G_CALLBACK(show_change_avatar_menu), button);
    g_signal_connect(change_password_button, "clicked", G_CALLBACK(show_change_password_menu), button);
    g_signal_connect(change_username_button, "clicked", G_CALLBACK(show_change_username_menu), button);
    g_signal_connect(logout_button, "clicked", G_CALLBACK(show_logout_menu), button);

    gtk_box_pack_start(GTK_BOX(box), change_avatar_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), change_password_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), change_username_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), logout_button, FALSE, FALSE, 5);

    gtk_container_add(GTK_CONTAINER(popover), box);
    gtk_widget_show_all(popover);
}

void show_chat_window(GtkApplication *app) {
    GtkWidget *window;
    GtkWidget *overlay;
    GtkWidget *fixed;
    GtkWidget *rect_left, *rect_right, *settings_button;
    GtkWidget *paned;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Bee Chat");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 800); 
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);  

    // Загружаем CSS стили
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "css/style.css", NULL);
    gtk_style_context_add_provider_for_screen(
        gtk_widget_get_screen(window),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    overlay = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(window), overlay);

    set_window_background_from_svg(overlay, "img/background.svg", 1920, 1080);

    // Создаем панель для левой и правой части
    paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), paned);

    // Левая панель
    rect_left = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_name(rect_left, "rect-left");
    gtk_widget_set_size_request(rect_left, 200, 100);

    GtkWidget *image = gtk_image_new_from_file("img/bee1.png");
    avatar_image_global = GTK_IMAGE(image);
    gtk_box_pack_start(GTK_BOX(rect_left), image, FALSE, FALSE, 0);

    GtkWidget *title_label = gtk_label_new(username);
    gtk_widget_set_name(title_label, "title-label");
    gtk_label_set_xalign(GTK_LABEL(title_label), 0.5);
    gtk_box_pack_start(GTK_BOX(rect_left), title_label, FALSE, FALSE, 5);

    GtkWidget *search_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_entry), "Search chat...");
    gtk_widget_set_name(search_entry, "search-entry");
    gtk_box_pack_start(GTK_BOX(rect_left), search_entry, FALSE, FALSE, 5);

    // Подключаем функцию для поиска
    g_signal_connect(search_entry, "changed", G_CALLBACK(on_search_changed), NULL);

    GtkWidget *scrollable_chat_list = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_chat_list),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_name(scrollable_chat_list, "chat-list-scroll");
    gtk_widget_set_size_request(scrollable_chat_list, 300, 500);

    chat_list = gtk_list_box_new();
    gtk_widget_set_name(chat_list, "chat-list-box");

    // Подключаем функцию для выбора чата
    g_signal_connect(chat_list, "row-selected", G_CALLBACK(on_chat_selected), NULL);

    gtk_container_add(GTK_CONTAINER(scrollable_chat_list), chat_list);
    gtk_box_pack_start(GTK_BOX(rect_left), scrollable_chat_list, TRUE, TRUE, 5);

    GtkWidget *create_chat_button = gtk_button_new_with_label("+ Create new chat");
    gtk_widget_set_name(create_chat_button, "create-chat-button");

    // Подключаем функцию для создания чата
    g_signal_connect(create_chat_button, "clicked", G_CALLBACK(on_create_chat_clicked), NULL);

    gtk_box_pack_end(GTK_BOX(rect_left), create_chat_button, FALSE, FALSE, 5);

    gtk_paned_add1(GTK_PANED(paned), rect_left);

    // Правая панель
    rect_right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *scrollable_message_area = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_message_area),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_name(scrollable_message_area, "message-area-scroll");

    message_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(scrollable_message_area), message_area);

    gtk_box_pack_start(GTK_BOX(rect_right), scrollable_message_area, TRUE, TRUE, 5);

    GtkWidget *entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    GtkWidget *file_button = gtk_button_new_with_label("📁");
    gtk_box_pack_start(GTK_BOX(entry_box), file_button, FALSE, FALSE, 5);

    // Подключаем функцию для выбора файла
    g_signal_connect(file_button, "clicked", G_CALLBACK(on_file_button_clicked), window);

    GtkWidget *message_entry = gtk_entry_new();
    GtkWidget *send_button = gtk_button_new_with_label("Send");

    gtk_box_pack_start(GTK_BOX(entry_box), message_entry, TRUE, TRUE, 5);
    gtk_box_pack_end(GTK_BOX(entry_box), send_button, FALSE, FALSE, 5);

    gtk_box_pack_end(GTK_BOX(rect_right), entry_box, FALSE, FALSE, 5);

    // Подключаем функцию для отправки сообщений
    g_signal_connect(send_button, "clicked", G_CALLBACK(add_message), message_entry);

    gtk_paned_add2(GTK_PANED(paned), rect_right);

    // Настройки
    fixed = gtk_fixed_new();
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), fixed);

    settings_button = gtk_button_new();
    GtkWidget *icon = gtk_image_new_from_icon_name("preferences-system", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(settings_button), icon);
    gtk_widget_set_name(settings_button, "settings_button");
    gtk_fixed_put(GTK_FIXED(fixed), settings_button, 5, 20);

    // Подключаем функцию для кнопки настроек
    g_signal_connect(settings_button, "clicked", G_CALLBACK(on_settings_clicked), NULL);

    gtk_widget_show_all(window);
}

