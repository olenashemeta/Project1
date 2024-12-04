#include "../inc/client.h"

void handle_login_response(cJSON *json_payload) {
    cJSON *status = cJSON_GetObjectItemCaseSensitive(json_payload, "status");
    if (!cJSON_IsBool(status)) {
        fprintf(stderr, "Missing or invalid 'status' in JSON data\n");
        return;
    }

    if (status->valueint) {
        printf("Login was successful\n");

        cJSON *data = cJSON_GetObjectItemCaseSensitive(json_payload, "data");
        if (!data) {
            fprintf(stderr, "Missing 'data' in JSON response\n");
            return;
        }

        t_bee_user user;
        memset(&user, 0, sizeof(t_bee_user));

        cJSON *login = cJSON_GetObjectItemCaseSensitive(data, "login");
        cJSON *username = cJSON_GetObjectItemCaseSensitive(data, "username");
        cJSON *created_at = cJSON_GetObjectItemCaseSensitive(data, "created_at");
        cJSON *logo_id = cJSON_GetObjectItemCaseSensitive(data, "logo_id");

        if (cJSON_IsString(login)) {
            size_t decoded_len;
            unsigned char *decoded = base64_decode(login->valuestring, &decoded_len);
            if (decoded) {
                user.login = strndup((char *)decoded, decoded_len);
                free(decoded);
            }
        }

        if (cJSON_IsString(username)) {
            size_t decoded_len;
            unsigned char *decoded = base64_decode(username->valuestring, &decoded_len);
            if (decoded) {
                user.username = strndup((char *)decoded, decoded_len);
                free(decoded);
            }
        }

        if (cJSON_IsString(created_at)) {
            size_t decoded_len;
            unsigned char *decoded = base64_decode(created_at->valuestring, &decoded_len);
            if (decoded) {
                user.created_at = strndup((char *)decoded, decoded_len);
                free(decoded);
            }
        }

        if (cJSON_IsNumber(logo_id)) {
            user.logo_id = logo_id->valueint;
        }

        printf("User login: %s, username: %s, created at: %s, logo_id: %d\n",
               user.login, user.username, user.created_at, user.logo_id);

        free(user.login);
        free(user.username);
        free(user.created_at);

    } else {
        printf("Login failed\n");

        cJSON *error_message = cJSON_GetObjectItemCaseSensitive(json_payload, "data");
        if (cJSON_IsString(error_message)) {
            printf("Error: %s\n", error_message->valuestring);
        }
    }
}

void handle_register_respone(cJSON *json_payload) {
    cJSON *status = cJSON_GetObjectItemCaseSensitive(json_payload, "status");
        if (!cJSON_IsBool(status)) {
        fprintf(stderr, "Missing or invalid 'status' in JSON data\n");
        return;
    }

    if (status->valueint) {

    }
}
