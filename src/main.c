#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>

void main(int argc, char **argv) {
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

    printf("Hashed password: %s\n", hashed);

    return 0;
}