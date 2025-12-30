#include <account.h>
#include <responses.h>
#include <mongoose.h>
#include <cjson/cJSON.h>
#include <password.h>

void handle_auth_request(struct mg_connection *connection, cJSON *request_json) {
    cJSON *pass = cJSON_GetObjectItem(request_json, "password");
    cJSON *username = cJSON_GetObjectItem(request_json, "username");
    if (!cJSON_IsString(pass) || !cJSON_IsString(username)) {
        invalid_request_res(connection);
        cJSON_Delete(request_json);
        return;
    }

    if (verify_password(pass->valuestring, generate_hash("teehee"))) {
        printf("Password verified successfully.\n");
    } else {
        printf("Password verification failed.\n");
    }
}