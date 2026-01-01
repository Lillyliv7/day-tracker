#include <responses.h>

#include <mongoose.h>

void return_status_code(struct mg_connection *connection, int code) {
    mg_http_reply(connection, code, CORS_HEADERS, ""); 
}