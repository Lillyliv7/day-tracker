#pragma once

#include <mongoose.h>
#include <cjson/cJSON.h>

void handle_auth_request(struct mg_connection *connection, cJSON *request_json);