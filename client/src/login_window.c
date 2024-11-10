#include "../inc/client.h"

static void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    GtkLabel *label = GTK_LABEL(user_data);

    t_main *main = (t_main *)g_object_get_data(G_OBJECT(button), "main_data");
    if (!main || main->socket < 0) {
        gtk_label_set_text(label, "No connection to server");
        return;
    }

    t_user *user = mx_create_client();
    printf("Created test client:\n");
    mx_print_client(user);

    if (generate_aes_key_iv(main->aes_key, main->aes_iv) != 0) {
        fprintf(stderr, "Failed to generate AES key and IV\n");
        return;
    }

    char aes_key_hex[AES_KEY_SIZE * 2 + 1];
    char iv_hex[AES_IV_SIZE * 2 + 1];
    bytes_to_hex_string(main->aes_key, AES_KEY_SIZE, aes_key_hex);
    bytes_to_hex_string(main->aes_iv, AES_IV_SIZE, iv_hex);

    unsigned char ciphertext[128];
    int encrypted_len = aes_encrypt((unsigned char *)user->password,
                                    strlen(user->password),
                                    main->aes_key, main->aes_iv, ciphertext);
    if (encrypted_len == -1) {
        fprintf(stderr, "Password encryption failed\n");
        return;
    }

    char encrypted_password_hex[encrypted_len * 2 + 1];
    bytes_to_hex_string(ciphertext, encrypted_len, encrypted_password_hex);

    unsigned char encrypted_aes_key[256];
    int encrypted_key_len = encrypt_aes_key(main->pubkey, main->aes_key, encrypted_aes_key);
    if (encrypted_key_len == -1) {
        fprintf(stderr, "AES key encryption failed\n");
        return;
    }

    char encrypted_aes_key_hex[encrypted_key_len * 2 + 1];
    bytes_to_hex_string(encrypted_aes_key, encrypted_key_len, encrypted_aes_key_hex);

    cJSON *login_request = form_login_request(user->login, ciphertext, encrypted_len, encrypted_aes_key, encrypted_key_len, main->aes_iv);
    if (!login_request) {
        fprintf(stderr, "Failed to create login request JSON\n");
        return;
    }
    char *json_str = cJSON_PrintUnformatted(login_request);
    if (!json_str) {
        fprintf(stderr, "Failed to print JSON\n");
        cJSON_Delete(login_request);
        return;
    }

    printf("Login request JSON: %s\n", json_str);

    if (send(main->socket, json_str, strlen(json_str), 0) == -1) {
        perror("Failed to send login request to server");
    }

    free(json_str);
    cJSON_Delete(login_request);
    mx_free_client(user);
}
 
void login_window(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *window_box;
    GtkWidget *button;
    GtkWidget *button_box;
    GtkWidget *status_label;
    t_main *main = (t_main *)user_data;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "CHAT");
    gtk_window_set_default_size(GTK_WINDOW(window), 480, 900);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    
    window_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(window), window_box);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(window_box), button_box, TRUE, TRUE, 0);

    button = gtk_button_new_with_label("Log in");
    gtk_container_add(GTK_CONTAINER(button_box), button);

    status_label = gtk_label_new("Status: Disconnected");
    gtk_box_pack_start(GTK_BOX(window_box), status_label, TRUE, TRUE, 0);

    g_object_set_data(G_OBJECT(button), "main_data", main);

    g_signal_connect(button, "clicked", G_CALLBACK(on_login_button_clicked), status_label);
    gtk_widget_show_all(window);
}
