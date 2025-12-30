#include <stdio.h>
#include <stdlib.h>

#include <cjson/cJSON.h>

#include <fnvhash.h>
#include <password.h>
#include <api.h>
#include <config.h>

int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <config-file>\n", argv[0]);
        return 1;
    }

    // cJSON *json = cJSON_Parse("{\"name\":\"test\",\"value\":123}");

    // puts(cJSON_Print(json));

    if (!config_parse(argv[1])) {
        fprintf(stderr, "Failed to parse config file: %s\n", argv[1]);
        return 1;
    }
    // puts(cJSON_Print(get_config_json()));

    // puts(get_config_string("listen_address"));

    // printf("%lx\n", fnv1a_hash("你好世界"));

    // if (verify_password("teehee", generate_hash("teehee"))) {
    //     printf("Password verified successfully.\n");
    // } else {
    //     printf("Password verification failed.\n");
    // }

    api_init();

    return 0;
}
