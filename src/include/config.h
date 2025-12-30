#pragma once

#include <cjson/cJSON.h>

cJSON *get_config_json();
int config_parse(const char *filename);