#pragma once

#include <stdbool.h>

#include <mongoose.h>
#include <cjson/cJSON.h>
bool account_exists(const char *username);
char* get_account_path(const char *username);
char *fetch_user_data(const char *username);
bool set_user_data(const char *username, const char *json);

void handle_auth_request(struct mg_connection *connection, cJSON *request_json);