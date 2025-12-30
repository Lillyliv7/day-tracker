#include <config.h>

#include <cjson/cJSON.h>

#include <stdio.h>
#include <stdlib.h>
cJSON *config_json;

cJSON *get_config_json() {
    return config_json;
}

const char* get_config_string(const char *key) {
    cJSON *item = cJSON_GetObjectItem(config_json, key);
    if (cJSON_IsString(item)) {
        return item->valuestring;
    }
    return NULL;
}

int config_parse(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open config file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    // printf("%ld\n", length);
    if (!data) {
        perror("Failed to allocate memory for config file");
        fclose(file);
        return 0;
    }

    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    puts(data);

    config_json = cJSON_Parse(data);

    if (!config_json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        fprintf(stderr, "Failed to parse config JSON\n");
        free(data);
        return 0;
    }

    free(data);

    return 1;

}