#include <api.h>

#include <password.h>

#include <config.h>

#include <mongoose.h>
#include <cjson/cJSON.h>

void invalid_request_res(struct mg_connection *connection) {
    mg_http_reply(connection, 400, "", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Invalid request")); 
}

void ratelimit_request_res(struct mg_connection *connection) {
    mg_http_reply(connection, 429, "", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Ratelimit")); 
}

void invalid_password_request_res(struct mg_connection *connection) {
    mg_http_reply(connection, 403, "", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Forbidden")); 
}

void handle_auth_request(struct mg_connection *connection, cJSON *request_json) {
    cJSON *pass = cJSON_GetObjectItem(request_json, "password");
    cJSON *username = cJSON_GetObjectItem(request_json, "username");
    if (!cJSON_IsString(pass) || !cJSON_IsString(username)) {
        invalid_request_res(connection);
        cJSON_Delete(request_json);
        return;
    }

    if (verify_password(pass->valuestring, generate_hash("teehee"))) {
        printf("Password verified successfully.\n");
    } else {
        printf("Password verification failed.\n");
    }
}

void handle_event(struct mg_connection *connection, int ev, void *ev_data) {
    if(ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *msg = (struct mg_http_message *) ev_data;
        puts(msg->body.buf);
        if (msg->body.len == 0) {
            invalid_request_res(connection);
            return;
        }
        if (msg->body.len > 1000000) {
            ratelimit_request_res(connection);
            return;
        }

        cJSON *request_json = cJSON_Parse(msg->body.buf);
        if (request_json == NULL) {
            invalid_request_res(connection);
            cJSON_Delete(request_json);
            return;
        }
        cJSON *request_type = cJSON_GetObjectItem(request_json, "type");
        if (!cJSON_IsString(request_type)) {
            invalid_request_res(connection);
            cJSON_Delete(request_json);
            return;
        }
        if (!strcmp(request_type->valuestring, "auth")) {
            handle_auth_request(connection, request_json);
            cJSON_Delete(request_json);
            return;
        }

        mg_http_reply(connection, 500, "", "{%m:%m}\n", MG_ESC("error"), MG_ESC("teehee"));
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
    puts(listen_address);
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, listen_address, handle_event, NULL);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
}

