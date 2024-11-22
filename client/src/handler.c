
/*
#include "../inc/client.h"

static void *connection(void *arg) {
    t_main *main = (t_main *)arg;

    while (!main->is_closing) {
        // 1. Подключение к серверу
        main->socket = mx_connect_to_server(main);
        if (main->socket == -1) {
            printf("Failed to connect to server. Retrying in %d seconds...\n", main->rec_delay);
            sleep(main->rec_delay);
            continue;
        }

        printf("Connected to server.\n");
        main->is_connected = true;

        // 2. Получение публичного ключа
        if (main->pubkey) {
            EVP_PKEY_free(main->pubkey); // Освобождаем старый ключ, если он существует
            main->pubkey = NULL;
        }

        if (mx_receiving_pubkey(main->socket, &main->pubkey) == -1) {
            perror("Failed to receive public key from server. Retrying connection...");
            close(main->socket);
            main->is_connected = false;
            continue; // Переподключение
        }

        printf("Received public key.\n");
        if (!PEM_write_PUBKEY(stdout, main->pubkey)) {
            fprintf(stderr, "Failed to write public key\n");
            ERR_print_errors_fp(stderr);
            close(main->socket);
            main->is_connected = false;
            continue; // Переподключение
        }

        // 3. Отправка существующего AES-ключа и IV
        if (mx_transfer_aes_key(main->aes_key, main->aes_iv, main->socket, main->pubkey) != 0) {
            fprintf(stderr, "Failed to transfer AES key to server. Retrying connection...\n");
            close(main->socket);
            main->is_connected = false;
            continue; // Переподключение
        }

        printf("AES key and IV successfully transferred to server.\n");

        // 4. Основной цикл обработки сообщений от сервера
        while (!main->is_closing) {
            char buffer[4096];
            ssize_t bytes_received = recv(main->socket, buffer, sizeof(buffer) - 1, 0);

            // Если соединение разорвано
            if (bytes_received <= 0) {
                fprintf(stderr, "Connection lost. Reconnecting...\n");
                close(main->socket);
                main->is_connected = false;
                break; // Выйти и переподключиться
            }

            buffer[bytes_received] = '\0';

            // Обработка JSON-ответа
            cJSON *json_response = cJSON_Parse(buffer);
            if (!json_response) {
                fprintf(stderr, "Failed to parse JSON response from server.\n");
                continue;
            }

            // Сохранение ответа в основном потоке
            pthread_mutex_lock(&main->lock);
            cJSON_Delete(main->server_response);
            main->server_response = json_response;
            main->has_new_data = true;
            pthread_cond_signal(&main->cond);
            pthread_mutex_unlock(&main->lock);
        }

        // Закрытие сокета перед повторным подключением
        close(main->socket);
        main->socket = -1;
    }

    printf("Closing connection thread.\n");
    pthread_exit(NULL);
}
*/

