#pragma once

#include <mongoose.h>
#include <cjson/cJSON.h>

char *fetch_user_data(const char *username);
char *set_user_data(const char *username, const char *json);

void handle_auth_request(struct mg_connection *connection, cJSON *request_json);