#pragma once

#include <cjson/cJSON.h>

cJSON *get_config_json();
const char* get_config_string(const char *key);
int config_parse(const char *filename);