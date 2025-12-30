#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <crypt.h>

bool verify_password(const char* password, const char* hash) {
    char *calculated_hash = crypt(password, hash);
    // printf("Calculated hash: %s\n", calculated_hash);
    if (calculated_hash == NULL) return false;
    return strcmp(calculated_hash, hash) == 0;
}

int main(int argc, char **argv) {
    const char *password = "teehee";

    const char *salt = crypt_gensalt("$2a$", 12, NULL, 0);
    if (salt == NULL) {
        fprintf(stderr, "Error generating salt\n");
        exit(EXIT_FAILURE);
    }
    char *hashed = crypt(password, salt);
    if (hashed == NULL) {
        fprintf(stderr, "Error hashing password\n");
        exit(EXIT_FAILURE);
    }
    hashed = strdup(hashed);

    // printf("Hashed password: %s\n", hashed);

    if (verify_password("teehee", hashed)) {
        printf("Password verified successfully.\n");
    } else {
        printf("Password verification failed.\n");
    }


    return 0;
}