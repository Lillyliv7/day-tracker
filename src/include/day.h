#pragma once

#include <stdbool.h>

#include <mongoose.h>
#include <cjson/cJSON.h>

bool delete_day(const char *username, int year, int month, int day);
bool day_exists(const char *username, int year, int month, int day);
bool add_day(const char *username, int year, int month, int day, int score, const char *note);
void handle_add_day_request(struct mg_connection *connection, cJSON *request_json);
void handle_delete_day_request(struct mg_connection *connection, cJSON *request_json);
void handle_get_days_request (struct mg_connection *connection, cJSON *request_json);