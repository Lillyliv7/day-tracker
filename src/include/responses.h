#pragma once

#include <mongoose.h>


void invalid_request_res(struct mg_connection *connection);
void ratelimit_request_res(struct mg_connection *connection);
void invalid_password_request_res(struct mg_connection *connection);
void unauthorized_request_res(struct mg_connection *connection);
void server_error_res(struct mg_connection *connection);
void success_res(struct mg_connection *connection);
