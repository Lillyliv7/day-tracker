#pragma once

#include <stdbool.h>

#include <mongoose.h>
#include <cjson/cJSON.h>

#define TOKEN_EXPIRY_SECONDS 2592000 // 30 days

bool account_exists(const char *username);
char* get_account_path(const char *username);
char *fetch_user_data(const char *username);
bool set_user_data(const char *username, const char *json);

char* account_create_token(cJSON *user_json);
bool verify_token(const char *username, const char *token);
char* check_account_token(const char *username);

bool delete_account(const char *username);
bool create_account(const char *username, const char *password);

void handle_delete_account_request(struct mg_connection *connection, cJSON *request_json);
void handle_create_account_request(struct mg_connection *connection, cJSON *request_json);
void handle_auth_request(struct mg_connection *connection, cJSON *request_json);