#pragma once

#include <cjson/cJSON.h>

void cJSON_SetString(cJSON *object, const char *name, const char *value);
void cJSON_SetInt(cJSON *object, const char *name, const long long value);