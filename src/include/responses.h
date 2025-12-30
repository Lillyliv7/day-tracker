#pragma once

#include <mongoose.h>


void invalid_request_res(struct mg_connection *connection);
void ratelimit_request_res(struct mg_connection *connection);
void invalid_password_request_res(struct mg_connection *connection);