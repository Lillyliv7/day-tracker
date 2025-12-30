#pragma once

#include <mongoose.h>

typedef enum {
    REQUEST_AUTH,
    REQUEST_CREATE_ACCOUNT,
} request_type_t;

void invalid_request(struct mg_connection *connection);
void ratelimit_request_res(struct mg_connection *connection);
void invalid_password_request_res(struct mg_connection *connection);

void handle_event(struct mg_connection *c, int ev, void *ev_data);

void api_init();
