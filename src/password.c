#include <password.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <crypt.h>



bool verify_password(const char *password, const char *hash) {
    char *calculated_hash = strdup(crypt(password, hash));
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
    char *hashed = strdup(crypt(password, salt));
    if (hashed == NULL) {
        fprintf(stderr, "Error hashing password\n");
        return NULL;
    }
    return hashed;
}
