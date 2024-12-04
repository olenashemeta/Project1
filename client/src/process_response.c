#include "../inc/client.h"

void process_response(t_main *main_data) {
    if (!main_data || !main_data->server_response) {
        fprintf(stderr, "Error: Invalid main_data or server_response in process_response\n");
        return;
    }

    cJSON *json_payload = main_data->server_response;
    cJSON *response_type = cJSON_GetObjectItemCaseSensitive(json_payload, "response_type");

    cJSON *status = cJSON_GetObjectItemCaseSensitive(json_payload, "status");
    main_data->status = cJSON_IsTrue(status);
    
    if (!cJSON_IsString(response_type) || !response_type->valuestring) {
        fprintf(stderr, "Missing or invalid 'response_type' in JSON data\n");
        return;
    }

    if (strcmp(response_type->valuestring, "login") == 0) {
        printf("A 'login' type response was received\n");
        handle_login_response(json_payload);
    }
}

