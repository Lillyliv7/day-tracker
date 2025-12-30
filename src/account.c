#include <account.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <responses.h>
#include <password.h>
#include <token.h>
#include <fnvhash.h>
#include <config.h>

#include <mongoose.h>
#include <cjson/cJSON.h>

bool account_exists(const char *username) {
    char* path = get_account_path(username);
    FILE *file = fopen(path, "rb");
    free(path);
    if (!file) {
        return false;
    }
    fclose(file);
    return true;
}

char* get_account_path(const char *username) {
    char* path = (char*) malloc(1024);
    sprintf(path, "%s/%lx.json", get_config_string("account_db_path"), fnv1a_hash(username));
    return path;
}

char *fetch_user_data(const char *username) {
    char* path = get_account_path(username);

    FILE *file = fopen(path, "rb");
    free(path);
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    // printf("%ld\n", length);
    if (!data) {
        perror("Failed to allocate memory for config file");
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);
    return data;
}

bool set_user_data(const char *username, const char *json) {
    char* path = get_account_path(username);

    FILE *file = fopen(path, "w");
    free(path);
    if (!file) return false;

    fwrite(json, 1, strlen(json), file);
    fclose(file);
    return true;
}

void handle_auth_request(struct mg_connection *connection, cJSON *request_json) {
    cJSON *pass = cJSON_GetObjectItem(request_json, "password");
    cJSON *username = cJSON_GetObjectItem(request_json, "username");
    if (!cJSON_IsString(pass) || !cJSON_IsString(username)) {
        invalid_request_res(connection);
        return;
    }

    if (!account_exists(username->valuestring)) {
        unauthorized_request_res(connection);
        return;
    }



    if (verify_password(pass->valuestring, generate_hash("teehee"))) {
        mg_http_reply(connection, 200, "", "{%m:%m}\n", MG_ESC("status"), MG_ESC("success"));
        return;
    } else {
        unauthorized_request_res(connection);
        return;
    }
}