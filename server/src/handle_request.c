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

    cJSON *password_item = cJSON_GetObjectItemCaseSensitive(json_payload, "password");
    if (!cJSON_IsString(password_item) || !password_item->valuestring) {
        syslog(LOG_ERR, "Missing or invalid 'password' in login request");
        return;
    }

    const char *userlogin = login_item->valuestring;
    const char *password = password_item->valuestring;

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "response_type", "login");
    t_user * user = db_user_read_by_login(userlogin);

    if (!user) {
        cJSON_AddBoolToObject(json, "status", false);
        cJSON_AddStringToObject(json, "data", "Couldn't find a user with this login.");
    }

    else {
        if(strcmp(user->password, password) == 0) {
            cJSON *json_user = user_to_json(user);
            if(!json_user) {
                cJSON_AddBoolToObject(json, "status", false);
                cJSON_AddStringToObject(json, "data", "Server error.");
            }
            else {
                cJSON_AddBoolToObject(json, "status", true);
                cJSON_AddItemToObject(json, "data", json_user);
                client->id_db = user->id;
            }
        }
        else {
            cJSON_AddBoolToObject(json, "status", false);
            cJSON_AddStringToObject(json, "data", "Wrong password.");
        }
        free_user(&user);
    }
    prepare_and_send_json(json, client);

    syslog(LOG_INFO, "Login request received. Userlogin: %s, Userpassword: %s", userlogin, password);
    
}

void handle_register_request(cJSON *json_payload, t_client *client) {

    if (!json_payload) {
        syslog(LOG_ERR, "Invalid JSON payload in handle_register_request");
        return;
    }

    cJSON *login_item = cJSON_GetObjectItemCaseSensitive(json_payload, "userlogin");
    if (!cJSON_IsString(login_item) || !login_item->valuestring) {
        syslog(LOG_ERR, "Missing or invalid 'userlogin' in register request");
        return;
    }

    cJSON *username_item = cJSON_GetObjectItemCaseSensitive(json_payload, "username");
    if (!cJSON_IsString(username_item) || !username_item->valuestring) {
        syslog(LOG_ERR, "Missing or invalid 'username' in register request");
        return;
    }
    
    cJSON *password_item = cJSON_GetObjectItemCaseSensitive(json_payload, "password");
    if (!cJSON_IsString(password_item) || !password_item->valuestring) {
        syslog(LOG_ERR, "Missing or invalid 'password' in register request");
        return;
    }

    const char *user_login = login_item->valuestring;
    const char *username = username_item->valuestring;
    const char *user_password = password_item->valuestring;

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "response_type", "register");

    t_user* user = user_create(username, user_login, user_password, 1);
    int user_id = db_user_create(t_user* user);
  
    if(user_id < 0) {
        cJSON_AddBoolToObject(json, "status", false);
        cJSON_AddStringToObject(json, "data", "Could not record user data.");
    }
    else {
        cJSON *json_user = user_to_json(user);
        if(!json_user) {
             cJSON_AddBoolToObject(json, "status", false);
             cJSON_AddStringToObject(json, "data", "Server error.");
        }
        else {
            client->id_db = user_id;
            cJSON_AddBoolToObject(json, "status", true);
            cJSON_AddItemToObject(json, "data", json_user);
        }
        free_user(&user) ;
    }

    prepare_and_send_json(json, client);

    syslog(LOG_INFO, "Register request received. Login: %s, Username: %s, Password: %s", user_login, username, user_password);
}
