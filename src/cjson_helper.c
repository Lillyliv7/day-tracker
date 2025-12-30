#include <cjson_helper.h>

#include <cjson/cJSON.h>

void cJSON_SetString(cJSON *object, const char *name, const char *value) {
    if (cJSON_HasObjectItem(object, name)) {
        cJSON_ReplaceItemInObject(object, name, cJSON_CreateString(value));
    } else {
        cJSON_AddStringToObject(object, name, value);
    }
}

void cJSON_SetInt(cJSON *object, const char *name, const long long value) {
    if (cJSON_HasObjectItem(object, name)) {
        cJSON_ReplaceItemInObject(object, name, cJSON_CreateNumber(value));
    } else {
        cJSON_AddNumberToObject(object, name, value);
    }
}