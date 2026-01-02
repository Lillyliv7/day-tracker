#include <api.h>

#include <password.h>
#include <responses.h>
#include <config.h>
#include <account.h>
#include <day.h>

#include <mongoose.h>
#include <cjson/cJSON.h>

void handle_event(struct mg_connection *connection, int ev, void *ev_data) {
    if(ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *msg = (struct mg_http_message *) ev_data;

        // puts(msg->method.buf);
        // printf("%d\n",msg->method.len);
        // puts(msg->body.buf);
        
        if (mg_strcmp(msg->method, mg_str("GET")) == 0) {
            struct mg_http_serve_opts opts = { .root_dir = get_config_string("www_path")};
            mg_http_serve_dir(connection, msg, &opts);
            return;
        }

        // allow cors
        if (mg_strcmp(msg->method, mg_str("OPTIONS")) == 0) {
            return_status_code(connection, 204);
            return;
        }

        // if the request is not a post, return invalid request
        if (mg_strcmp(msg->method, mg_str("POST"))) {
            return_status_code(connection, 400);
            return;
        }

        cJSON *request_json = cJSON_Parse(msg->body.buf);
        if (request_json == NULL) {
            return_status_code(connection, 400);
            cJSON_Delete(request_json);
            return;
        }
        cJSON *request_type = cJSON_GetObjectItem(request_json, "type");
        if (!cJSON_IsString(request_type)) {
            return_status_code(connection, 400);
            cJSON_Delete(request_json);
            return;
        }


        if (!strcmp(request_type->valuestring, "account")) {
            handle_account_request(connection, request_json);
            cJSON_Delete(request_json);
            return;
        }

        if (!strcmp(request_type->valuestring, "day")) {
            handle_day_request(connection, request_json);
            cJSON_Delete(request_json);
            return;
        }

        return_status_code(connection, 404);
        cJSON_Delete(request_json);
    }
}

void api_init() {
    struct mg_mgr mgr;
    const char* listen_address = get_config_string("listen_address");
    if (listen_address == NULL) {
        fprintf(stderr, "\"listen_address\" not found in config!\n");
        exit(1);
    }
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, listen_address, handle_event, NULL);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
}

