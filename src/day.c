#include <day.h>

#include <cjson/cJSON.h>
#include <mongoose.h>

#include <stdbool.h>

#include <responses.h>
#include <account.h>
#include <cjson_helper.h>

bool delete_day(const char *username, int year, int month, int day) {
    char *user_data = fetch_user_data(username);
    if (user_data == NULL) {
        return NULL;
    }
    cJSON *user_json = cJSON_Parse(user_data);
    free(user_data);
    if (user_json == NULL) {
        return NULL;
    }

    cJSON *days = cJSON_GetObjectItem(user_json, "days");
    if (!cJSON_IsArray(days)) return false;

    for (int i = 0; i < cJSON_GetArraySize(days); i++) {
        cJSON *day_obj = cJSON_GetArrayItem(days, i);
        cJSON *day_year = cJSON_GetObjectItem(day_obj, "year");
        cJSON *day_month = cJSON_GetObjectItem(day_obj, "month");
        cJSON *day_day = cJSON_GetObjectItem(day_obj, "day");
        if (!cJSON_IsNumber(day_year) || !cJSON_IsNumber(day_month) || !cJSON_IsNumber(day_day))
            continue;

        if (day_year->valueint == year && day_month->valueint == month && day_day->valueint == day) {
            cJSON_DeleteItemFromArray(days, i);
            set_user_data(username, user_json);
            cJSON_Delete(user_json);
            return true;

        }
    }
    cJSON_Delete(user_json);
    return false;
}

bool day_exists(const char *username, int year, int month, int day) {
    char *user_data = fetch_user_data(username);
    if (user_data == NULL) {
        return NULL;
    }
    cJSON *user_json = cJSON_Parse(user_data);
    free(user_data);
    if (user_json == NULL) {
        return NULL;
    }

    cJSON *days = cJSON_GetObjectItem(user_json, "days");
    if (!cJSON_IsArray(days)) return false;

    for (int i = 0; i < cJSON_GetArraySize(days); i++) {
        cJSON *day_obj = cJSON_GetArrayItem(days, i);
        cJSON *day_year = cJSON_GetObjectItem(day_obj, "year");
        cJSON *day_month = cJSON_GetObjectItem(day_obj, "month");
        cJSON *day_day = cJSON_GetObjectItem(day_obj, "day");
        if (!cJSON_IsNumber(day_year) || !cJSON_IsNumber(day_month) || !cJSON_IsNumber(day_day))
            continue;

        if (day_year->valueint == year && day_month->valueint == month && day_day->valueint == day) {
            cJSON_Delete(user_json);
            return true;
        }
    }
    cJSON_Delete(user_json);
    return false;
}

bool add_day(const char *username, int year, int month, int day, int score, const char *note) {
    char *user_data = fetch_user_data(username);
    if (user_data == NULL) {
        return NULL;
    }
    cJSON *user_json = cJSON_Parse(user_data);
    free(user_data);
    if (user_json == NULL) {
        return NULL;
    }

    if (day_exists(username, year, month, day)) {
        cJSON_Delete(user_json);
        return false;
    }

    cJSON *days = cJSON_GetObjectItem(user_json, "days");
    if (!cJSON_IsArray(days))
        days = cJSON_AddArrayToObject(user_json, "days");
    cJSON *day_entry = cJSON_CreateObject();
    cJSON_SetInt(day_entry, "year", year);
    cJSON_SetInt(day_entry, "month", month);
    cJSON_SetInt(day_entry, "day", day);
    cJSON_SetInt(day_entry, "score", score);
    cJSON_SetString(day_entry, "note", note);
    cJSON_AddItemToArray(days, day_entry);
    bool result = set_user_data(username, user_json);
    cJSON_Delete(user_json);

    return result;
}

void handle_add_day_request(struct mg_connection *connection, cJSON *request_json) {
    cJSON *token = cJSON_GetObjectItem(request_json, "token");
    cJSON *username = cJSON_GetObjectItem(request_json, "username");
    if (!cJSON_IsString(token) || !cJSON_IsString(username)) {
        invalid_request_res(connection);
        return;
    }
    if (!verify_token(username->valuestring, token->valuestring)) {
        unauthorized_request_res(connection);
        return;
    }
    cJSON *year = cJSON_GetObjectItem(request_json, "year");
    cJSON *month = cJSON_GetObjectItem(request_json, "month");
    cJSON *day = cJSON_GetObjectItem(request_json, "day");
    cJSON *score = cJSON_GetObjectItem(request_json, "score");
    cJSON *note = cJSON_GetObjectItem(request_json, "note");
    if (!cJSON_IsNumber(year) || !cJSON_IsNumber(month) || !cJSON_IsNumber(day) || !cJSON_IsNumber(score)) {
        invalid_request_res(connection);
        return;
    }
    if (!cJSON_IsString(note)) {
        invalid_request_res(connection);
        return;
    }
    if(!add_day(username->valuestring, year->valueint, month->valueint, day->valueint, score->valueint, note->valuestring)) {
        server_error_res(connection);
        return;
    }
    success_res(connection);
}

void handle_delete_day_request (struct mg_connection *connection, cJSON *request_json) {
    cJSON *token = cJSON_GetObjectItem(request_json, "token");
    cJSON *username = cJSON_GetObjectItem(request_json, "username");
    if (!cJSON_IsString(token) || !cJSON_IsString(username)) {
        invalid_request_res(connection);
        return;
    }
    if (!verify_token(username->valuestring, token->valuestring)) {
        unauthorized_request_res(connection);
        return;
    }
    cJSON *year = cJSON_GetObjectItem(request_json, "year");
    cJSON *month = cJSON_GetObjectItem(request_json, "month");
    cJSON *day = cJSON_GetObjectItem(request_json, "day");
    if (!cJSON_IsNumber(year) || !cJSON_IsNumber(month) || !cJSON_IsNumber(day)) {
        invalid_request_res(connection);
        return;
    }
    if(!delete_day(username->valuestring, year->valueint, month->valueint, day->valueint)) {
        server_error_res(connection);
        return;
    }
    success_res(connection);
}

void handle_get_days_request (struct mg_connection *connection, cJSON *request_json) {
    cJSON *token = cJSON_GetObjectItem(request_json, "token");
    cJSON *username = cJSON_GetObjectItem(request_json, "username");
    if (!cJSON_IsString(token) || !cJSON_IsString(username)) {
        invalid_request_res(connection);
        return;
    }
    if (!verify_token(username->valuestring, token->valuestring)) {
        unauthorized_request_res(connection);
        return;
    }

    char *user_data = fetch_user_data(username->valuestring);
    if (user_data == NULL) {
        return;
    }
    cJSON *user_json = cJSON_Parse(user_data);
    free(user_data);
    if (user_json == NULL) {
        return;
    }

    cJSON *days = cJSON_GetObjectItem(user_json, "days");
    if (!cJSON_IsArray(days))
        days = cJSON_AddArrayToObject(user_json, "days");
    

    mg_http_reply(connection, 200, "", "{%m:%m,%m:%m}\n", MG_ESC("status"), MG_ESC("Success"), MG_ESC("days"), MG_ESC(cJSON_Print(days)));
}
