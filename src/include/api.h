#pragma once

#include <mongoose.h>

typedef enum {
    REQUEST_AUTH,
    REQUEST_CREATE_ACCOUNT,
} request_type_t;

void handle_event(struct mg_connection *c, int ev, void *ev_data);

void api_init();
