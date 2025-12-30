#include <account.h>

#include <responses.h>
#include <password.h>
#include <token.h>
#include <fnvhash.h>

#include <mongoose.h>
#include <cjson/cJSON.h>

char *fetch_user_data(const char *username) {
    
}

char *set_user_data(const char *username, const char *json) {

}

void handle_auth_request(struct mg_connection *connection, cJSON *request_json) {
    cJSON *pass = cJSON_GetObjectItem(request_json, "password");
    cJSON *username = cJSON_GetObjectItem(request_json, "username");
    if (!cJSON_IsString(pass) || !cJSON_IsString(username)) {
        invalid_request_res(connection);
        cJSON_Delete(request_json);
        return;
    }

    if (verify_password(pass->valuestring, generate_hash("teehee"))) {
        
    } else {
        printf("Password verification failed.\n");
    }
}