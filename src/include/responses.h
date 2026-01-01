#pragma once

#include <mongoose.h>

#define CORS_HEADERS \
  "Access-Control-Allow-Origin: *\r\n" \
  "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n" \
  "Access-Control-Allow-Headers: Content-Type, Authorization, X-Requested-With\r\n"

void return_status_code(struct mg_connection *connection, int code);