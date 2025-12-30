#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <crypt.h>
#include <fnvhash.h>

bool verify_password(const char *password, const char *hash) {
    char *calculated_hash = crypt(password, hash);
    // printf("Calculated hash: %s\n", calculated_hash);
    if (calculated_hash == NULL) return false;
    return strcmp(calculated_hash, hash) == 0;
}

char *generate_hash(const char *password) {
    const char *salt = crypt_gensalt("$2a$", 12, NULL, 0);
    if (salt == NULL) {
        fprintf(stderr, "Error generating salt\n");
        return NULL;
    }
    char *hashed = crypt(password, salt);
    if (hashed == NULL) {
        fprintf(stderr, "Error hashing password\n");
        return NULL;
    }
    return strdup(hashed);
}



int main(int argc, char **argv) {


    printf("%lx\n", fnv1a_hash("helko"));

    // if (verify_password("teehee", generate_hash("teehee"))) {
    //     printf("Password verified successfully.\n");
    // } else {
    //     printf("Password verification failed.\n");
    // }


    return 0;
}