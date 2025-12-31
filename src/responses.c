#include <responses.h>

#include <mongoose.h>

void invalid_request_res(struct mg_connection *connection) {
    mg_http_reply(connection, 400, CORS_HEADERS"", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Invalid request")); 
}

void ratelimit_request_res(struct mg_connection *connection) {
    mg_http_reply(connection, 429, CORS_HEADERS"", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Ratelimit")); 
}

void invalid_password_request_res(struct mg_connection *connection) {
    mg_http_reply(connection, 403, CORS_HEADERS"", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Forbidden")); 
}

void unauthorized_request_res(struct mg_connection *connection) {
    mg_http_reply(connection, 401, CORS_HEADERS"", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Unauthorized")); 
}
void server_error_res(struct mg_connection *connection) {
    mg_http_reply(connection, 500, CORS_HEADERS"", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Internal Server Error")); 
}

void success_res(struct mg_connection *connection) {
    mg_http_reply(connection, 200, CORS_HEADERS"", "{%m:%m}\n", MG_ESC("status"), MG_ESC("Success")); 
}