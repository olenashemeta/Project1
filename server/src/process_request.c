#include "../inc/server.h"
//добавлен атрибут так как пока что структура t_client не используеться в этой функции
void process_request(t_packet *receive_data, t_client *client __attribute__((unused))) {
    if (!receive_data || !receive_data->data) {
        syslog(LOG_ERR, "Invalid t_receive structure in process_request");
        return;
    }

    cJSON *json_payload = cJSON_ParseWithLength(receive_data->data, receive_data->len);
    if (!json_payload) {
        syslog(LOG_ERR, "Failed to parse JSON from received data: %s", receive_data->data);
        return;
    }

    cJSON *request_type = cJSON_GetObjectItemCaseSensitive(json_payload, "request_type");
    if (!cJSON_IsString(request_type) || !request_type->valuestring) {
        syslog(LOG_ERR, "Missing or invalid 'request_type' in JSON data");
        cJSON_Delete(json_payload);
        return;
    }

    syslog(LOG_INFO, "Processing request of type: %s", request_type->valuestring);

    if (strcmp(request_type->valuestring, "login") == 0) {
        syslog(LOG_INFO, "Handling 'login' request");
        handle_login_request(json_payload);
    } else if (strcmp(request_type->valuestring, "registration")) {
        syslog(LOG_INFO, "Handling 'registration' request");
    }

    cJSON_Delete(json_payload);
}

