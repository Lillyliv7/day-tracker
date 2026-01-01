#include <account.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <responses.h>
#include <password.h>
#include <generate_token.h>
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

bool set_user_data(const char *username, cJSON *json) {
    char* json_string = cJSON_Print(json);

    char* path = get_account_path(username);

    FILE *file = fopen(path, "w");
    free(path);
    if (!file) return false;

    fwrite(json_string, 1, strlen(json_string), file);
    fclose(file);
    free(json_string);
    return true;
}

char* account_create_token(cJSON *user_json) {
    char *token = generate_token();
    cJSON_SetString(user_json, "token", token);
    cJSON_SetInt(user_json, "token_expiry", time(NULL) + TOKEN_EXPIRY_SECONDS);
    set_user_data(cJSON_GetObjectItem(user_json, "username")->valuestring, user_json);
    
    return token;
}

// this function is called after the user has been authenticated
// and is always supposed to return a token. If the token is out
// of date, it automatically creates a new one.
char* token_auth(const char *username) {
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
        char* token_string = account_create_token(user_json);
        cJSON_Delete(user_json);
        return token_string;
    }
    if (cJSON_GetObjectItem(user_json, "token_expiry")->valuedouble < time(NULL)) {
        char* token_string = account_create_token(user_json);
        cJSON_Delete(user_json);
        return token_string;
    }
    char* token_string = strdup(token->valuestring);
    cJSON_Delete(user_json);
    return token_string;
}

bool verify_token(const char *username, const char *token) {
    char *user_data = fetch_user_data(username);
    if (user_data == NULL) {
        return false;
    }
    cJSON *user_json = cJSON_Parse(user_data);
    free(user_data);
    if (user_json == NULL) {
        return false;
    }
    cJSON *stored_token = cJSON_GetObjectItem(user_json, "token");
    if (!cJSON_IsString(stored_token)) {
        cJSON_Delete(user_json);
        return false;
    }

    cJSON *token_expiry = cJSON_GetObjectItem(user_json, "token_expiry");
    if (!cJSON_IsNumber(token_expiry)) {
        cJSON_Delete(user_json);
        return false;
    }
    if (token_expiry->valuedouble < time(NULL)) {
        cJSON_Delete(user_json);
        return false;
    }

    bool result = (strcmp(stored_token->valuestring, token) == 0);
    cJSON_Delete(user_json);
    return result;
}

bool create_account(const char *username, const char *password) {
    if (account_exists(username)) {
        return false;
    }

    cJSON *user_json = cJSON_CreateObject();
    cJSON_AddStringToObject(user_json, "username", username);
    cJSON_AddStringToObject(user_json, "password", generate_hash(password));

    bool result = set_user_data(username, user_json);
    cJSON_Delete(user_json);
    return result;
}

bool delete_account(const char *username) {
    if (!account_exists(username)) return false;

    char* path = get_account_path(username);
    return (remove(path) == 0);
}


void handle_account_request(struct mg_connection *connection, cJSON *request_json) {

    cJSON *request_operation = cJSON_GetObjectItem(request_json, "operation");
    if (!cJSON_IsString(request_operation)) {
        return_status_code(connection, 400);
        return;
    }

    /* Check Token */

    if(!strcmp(request_operation->valuestring, "check_token")) {
        cJSON *token = cJSON_GetObjectItem(request_json, "token");
        cJSON *username = cJSON_GetObjectItem(request_json, "username");
        if (!cJSON_IsString(token) || !cJSON_IsString(username)) {
            return_status_code(connection, 400);
            return;
        }

        if (verify_token(username->valuestring, token->valuestring)) {
            return_status_code(connection, 200);
            return;
        } else {
            return_status_code(connection, 401);
            return;
        }
    }

    /* Delete Account */

    if(!strcmp(request_operation->valuestring, "delete")) {
        cJSON *pass = cJSON_GetObjectItem(request_json, "password");
        cJSON *username = cJSON_GetObjectItem(request_json, "username");
        if (!cJSON_IsString(pass) || !cJSON_IsString(username)) {
            return_status_code(connection, 400);
            return;
        }

        char* user_data = fetch_user_data(username->valuestring);
        if (!user_data) {
            return_status_code(connection, 404);
            return;
        }
        cJSON *account_json = cJSON_Parse(user_data);
        if (!account_json) {
            return_status_code(connection, 500);
            cJSON_Delete(account_json);
            return;
        }

        cJSON *account_password = cJSON_GetObjectItem(account_json, "password");
        if (!cJSON_IsString(account_password)) {
            return_status_code(connection, 500);
            return;
        }
        if (verify_password(pass->valuestring, account_password->valuestring)) {
            if (delete_account(username->valuestring)) {
                return_status_code(connection, 200);
                cJSON_Delete(account_json);
                return;
            } else {
                return_status_code(connection, 500);
                cJSON_Delete(account_json);
                return;
            }
        } else {
            return_status_code(connection, 401);
            cJSON_Delete(account_json);
            return;
        }
    }

    /* Create Account */

    if(!strcmp(request_operation->valuestring, "create")) {
        cJSON *pass = cJSON_GetObjectItem(request_json, "password");
        cJSON *username = cJSON_GetObjectItem(request_json, "username");
        if (!cJSON_IsString(pass) || !cJSON_IsString(username)) {
            return_status_code(connection, 400);
            return;
        }

        if (create_account(username->valuestring, pass->valuestring)) {
            return_status_code(connection, 200);
            return;
        } else {
            return_status_code(connection, 403);
            return;
        }
    }

    /* Authenticate Account */

    if(!strcmp(request_operation->valuestring, "auth")) {
        cJSON *pass = cJSON_GetObjectItem(request_json, "password");
        cJSON *username = cJSON_GetObjectItem(request_json, "username");
        if (!cJSON_IsString(pass) || !cJSON_IsString(username)) {
            return_status_code(connection, 400);
            return;
        }

        if (!account_exists(username->valuestring)) {
            return_status_code(connection, 404);
            return;
        }

        char* user_data = fetch_user_data(username->valuestring);
        if (!user_data) {
            return_status_code(connection, 404);
            return;
        }
        cJSON *account_json = cJSON_Parse(user_data);
        if (!account_json) {
            return_status_code(connection, 500);
            free(user_data);
            return;
        }

        cJSON *account_password = cJSON_GetObjectItem(account_json, "password");
        if (!cJSON_IsString(account_password)) {
            return_status_code(connection, 500);
            cJSON_Delete(account_json);
            free(user_data);
            return;
        }

        if (verify_password(pass->valuestring, account_password->valuestring)) {
            puts("verified");
            char* token = token_auth(username->valuestring);
            if (token == NULL) {
                return_status_code(connection, 500);
                cJSON_Delete(account_json);
                free(user_data);
                return;
            }
            mg_http_reply(connection, 200, CORS_HEADERS, "{%m:%m,%m:%m}\n", MG_ESC("status"), MG_ESC("success"), MG_ESC("token"), MG_ESC(token));
            free(token);
            cJSON_Delete(account_json);
            free(user_data);
            return;
        } else {
            return_status_code(connection, 401);
            cJSON_Delete(account_json);
            free(user_data);
            return;
        }

    }

    return_status_code(connection, 404);
    return;
}