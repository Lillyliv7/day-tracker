#pragma once

#include <mongoose.h>

void handle_request(struct mg_connection *c, int ev, void *ev_data);

void api_init();
