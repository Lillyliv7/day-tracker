#include <stdio.h>

#include <cjson/cJSON.h>

#include <fnvhash.h>
#include <password.h>
#include <api.h>


int main(int argc, char **argv) {
    cJSON *json = cJSON_Parse("{\"name\":\"test\",\"value\":123}");

    puts(cJSON_Print(json));

    // printf("%lx\n", fnv1a_hash("你好世界"));

    // if (verify_password("teehee", generate_hash("teehee"))) {
    //     printf("Password verified successfully.\n");
    // } else {
    //     printf("Password verification failed.\n");
    // }

    api_init();

    return 0;
}
