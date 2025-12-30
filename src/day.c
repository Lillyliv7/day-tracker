#include <day.h>

#include <cjson/cJSON.h>

#include <stdbool.h>

#include <responses.h>
#include <account.h>

bool add_day(const char *username, int year, int month, int day, int score, const char *note) {

    return true;
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
