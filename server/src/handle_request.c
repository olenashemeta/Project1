#include "../inc/server.h"

void handle_login_request(cJSON *json_payload) {
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

<<<<<<< HEAD
    t_user * user = user_create(userlogin, userlogin, password);
    db_user_create(user);
    free_user(&user);

=======
>>>>>>> origin
    syslog(LOG_INFO, "Login request received. Userlogin: %s, Userpassword: %s", userlogin, password);
    
}
