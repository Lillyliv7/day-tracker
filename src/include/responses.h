#pragma once

#include <mongoose.h>

#define CORS_HEADERS \
  "Access-Control-Allow-Origin: *\r\n" \
  "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n" \
  "Access-Control-Allow-Headers: Content-Type, Authorization, X-Requested-With\r\n"

void invalid_request_res(struct mg_connection *connection);
void ratelimit_request_res(struct mg_connection *connection);
void invalid_password_request_res(struct mg_connection *connection);
void unauthorized_request_res(struct mg_connection *connection);
void server_error_res(struct mg_connection *connection);
void success_res(struct mg_connection *connection);
