#include <account.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <responses.h>
#include <password.h>
#include <token.h>
#include <fnvhash.h>
#include <config.h>
#include <cjson_helper.h>

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

char* account_create_token(cJSON *user_json) {
    char *token = generate_token();
    // cJSON_AddStringToObject(user_json, "token", token);
    cJSON_SetString(user_json, "token", token);
    // cJSON_AddNumberToObject(user_json, "token_expiry", time(NULL) + TOKEN_EXPIRY_SECONDS);
    cJSON_SetInt(user_json, "token_expiry", time(NULL) + TOKEN_EXPIRY_SECONDS);
    char* json_string = cJSON_Print(user_json);
    set_user_data(cJSON_GetObjectItem(user_json, "username")->valuestring, json_string);
    free(json_string);
    
    return token;
}

char* check_account_token(const char *username) {
    char *user_data = fetch_user_data(username);
    if (user_data == NULL) {
        return NULL;
    }
    cJSON *user_json = cJSON_Parse(user_data);
    free(user_data);
    if (user_json == NULL) {
        return NULL;
    }
    cJSON *token = cJSON_GetObjectItem(user_json, "token");
    if (!cJSON_IsString(token)) {
        return account_create_token(user_json);
    }
    if (cJSON_GetObjectItem(user_json, "token_expiry")->valuedouble < time(NULL)) {
        return account_create_token(user_json);
    }
    return strdup(token->valuestring);
    
}

bool create_account(const char *username, const char *password) {
    if (account_exists(username)) {
        return false;
    }

    cJSON *user_json = cJSON_CreateObject();
    cJSON_AddStringToObject(user_json, "username", username);
    cJSON_AddStringToObject(user_json, "password", generate_hash(password));

    char* json_string = cJSON_Print(user_json);
    bool result = set_user_data(username, json_string);
    free(json_string);
    cJSON_Delete(user_json);
    return result;
}

void handle_create_account_request(struct mg_connection *connection, cJSON *request_json) {
    cJSON *pass = cJSON_GetObjectItem(request_json, "password");
    cJSON *username = cJSON_GetObjectItem(request_json, "username");
    if (!cJSON_IsString(pass) || !cJSON_IsString(username)) {
        invalid_request_res(connection);
        return;
    }
    if (create_account(username->valuestring, pass->valuestring)) {
        success_res(connection);
        return;
    } else {
        invalid_request_res(connection);
        return;
    }
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
        char* token = check_account_token(username->valuestring);
        if (token == NULL) {
            server_error_res(connection);
            return;
        }
        mg_http_reply(connection, 200, "", "{%m:%m,%m:%m}\n", MG_ESC("status"), MG_ESC("success"), MG_ESC("token"), MG_ESC(token));
        free(token);
        return;
    } else {
        unauthorized_request_res(connection);
        return;
    }
}