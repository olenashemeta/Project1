#include "../inc/server.h"

void mx_process_client_request(cJSON *json) {
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "request_type");
    if (!cJSON_IsString(type) || (type->valuestring == NULL)) {
        fprintf(stderr, "Invalid request type\n");
        return;
    }

    if (strcmp(type->valuestring, "register") == 0) {
        syslog(LOG_INFO, "Processing registration request");
        // registration processing
    }
    else if (strcmp(type->valuestring, "login") == 0) {
        syslog(LOG_INFO, "Processing login request");
        handle_login_request(json);
    }
    /*
    else if (strcmp(type->valuestring, "search_chats") == 0) {
        syslog(LOG_INFO, "Processing chat search request for client ID: %d", client->client_id);
    } 
    else if (strcmp(type->valuestring, "search_users") == 0) {
        syslog(LOG_INFO, "Processing user search request for client ID: %d", client->client_id);
    }
    */
}

