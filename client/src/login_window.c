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
    

    //cJSON *login_request = form_login_request_test(main->aes_key, main->aes_iv, user->login, user->password);
    //if (!login_request) {
    //    fprintf(stderr, "Failed to create login request JSON\n");
    //    return;
    //}

    //size_t encrypted_data_len;

    /*
    unsigned char *encrypted_data = encrypt_json(main->pubkey, login_request, &encrypted_data_len);
    if (!encrypted_data) {
        fprintf(stderr, "Failed to encrypt JSON object\n");
        return;
    }
    */
    //unsigned char *encrypted_data = encrypt_json_with_aes(main->aes_key, main->aes_iv, login_request, &encrypted_data_len);
    //if (!encrypted_data) {
    //    fprintf(stderr, "Failed to encrypt JSON object\n");
    //    return;
    //}

    //printf("Login request JSON: %s\n", encrypted_data);
    
    /*
    uint32_t data_len = htonl((uint32_t)encrypted_data_len);
    if (send(main->socket, &data_len, sizeof(data_len), 0) == -1) {
        perror("Failed to send data length to server");
    }

    if (send(main->socket, encrypted_data, encrypted_data_len, 0) == -1) {
        perror("Failed to send encrypted data to server");
    }
    */
    //free(encrypted_data);
    //cJSON_Delete(login_request);
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
