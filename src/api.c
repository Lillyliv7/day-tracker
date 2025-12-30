#include <api.h>

#include <config.h>

#include <mongoose.h>

void handle_request(struct mg_connection *c, int ev, void *ev_data) {
    if(ev == MG_EV_HTTP_MSG) {
        puts("Received HTTP request\n");
    }
}

void api_init() {
    struct mg_mgr *mgr;
    const char* listen_address = get_config_string("listen_address");
    if (listen_address == NULL) {
        fprintf(stderr, "\"listen_address\" not found in config!\n");
        exit(1);
    }
    mg_mgr_init(mgr);
    mg_http_listen(mgr, "http://0.0.0.0:8000", handle_request, NULL);
    for (;;) {
        mg_mgr_poll(mgr, 1000);
    }
}

