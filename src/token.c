#include <token.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char *generate_token() {
    uint8_t *tokenBuf = (uint8_t *)malloc(TOKEN_LENGTH);
    if (tokenBuf == NULL) {
        fprintf(stderr, "Failed to allocate memory for token!\n");
        return NULL;
    }

    FILE *rand = fopen("/dev/urandom", "rb");
    if (!rand) {
        perror("Failed to open config /dev/urandom!");
        return 1;
    }

    fseek(rand, 0, SEEK_END);
    fread(tokenBuf, 1, TOKEN_LENGTH, rand);
    fclose(rand);

    char *tokenStr = (char *)malloc(TOKEN_LENGTH * 2 + 1);
    if (tokenStr == NULL) {
        fprintf(stderr, "Failed to allocate memory for token string!\n");
        free(tokenBuf);
        return NULL;
    }
    for (size_t i = 0; i < TOKEN_LENGTH; i++) {
        sprintf(&tokenStr[i * 2], "%02x", tokenBuf[i]);
    }
    free(tokenBuf);
    
    puts(tokenStr);
    return tokenStr;
}
