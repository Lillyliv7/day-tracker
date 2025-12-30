#include <responses.h>

#include <mongoose.h>

void invalid_request_res(struct mg_connection *connection) {
    mg_http_reply(connection, 400, "", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Invalid request")); 
}

void ratelimit_request_res(struct mg_connection *connection) {
    mg_http_reply(connection, 429, "", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Ratelimit")); 
}

void invalid_password_request_res(struct mg_connection *connection) {
    mg_http_reply(connection, 403, "", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Forbidden")); 
}
