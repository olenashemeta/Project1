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
        t_bee_user *user = create_bee_user_from_json(data);
        if (!user) {
            fprintf(stderr, "Failed to create user from JSON\n");
            return;
        }

        printf("User login: %s, username: %s, created at: %s, logo_id: %d\n",
               user->login, user->username, user->created_at, user->logo_id);

        free_bee_user(user);

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
