#include <fnvhash.h>
#include <stdint.h>

uint64_t fnv1a_hash(const char* str) {
    uint64_t hash = 0xcbf29ce484222325;
    while (*str) {
        hash ^= (unsigned char)*str++;
        hash *= 0x100000001b3;
    }
    return hash;
}