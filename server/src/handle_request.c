#include "../inc/server.h"

void handle_login_request(cJSON *json_payload, t_client *client) {
    if (!json_payload) {
        syslog(LOG_ERR, "Invalid JSON payload in handle_login_request");
        return;
    }

    cJSON *login_item = cJSON_GetObjectItemCaseSensitive(json_payload, "userlogin");
    if (!cJSON_IsString(login_item) || !login_item->valuestring) {
        syslog(LOG_ERR, "Missing or invalid 'userlogin' in login request");
        return;
    }
    const char *userlogin = login_item->valuestring;

    cJSON *password_item = cJSON_GetObjectItemCaseSensitive(json_payload, "password");
    if (!cJSON_IsString(password_item) || !password_item->valuestring) {
        syslog(LOG_ERR, "Missing or invalid 'password' in login request");
        return;
    }
    const char *password = password_item->valuestring;
    
     cJSON *json = cJSON_CreateObject();
     cJSON_AddStringToObject(json, "response_type", "login");

    t_user * user = db_user_read_by_login();
    
    cJSON *json_user = user_to_json(user);
    if(!json_user) {
         cJSON_AddBoolToObject(json, "status", false);
         cJSON_AddStringToObject(json, "data", "User could not be found");
    }
    else {
        cJSON_AddBoolToObject(json, "status", true);
        cJSON_AddItemToObject(json, "data", json_user);
    }

    prepare_and_send_json(json, client);
    free_user(&user);

    syslog(LOG_INFO, "Login request received. Userlogin: %s, Userpassword: %s", userlogin, password);
    
}
