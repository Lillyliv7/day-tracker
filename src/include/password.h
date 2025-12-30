#pragma once

#include <stdbool.h>

bool verify_password(const char *password, const char *hash);

char *generate_hash(const char *password);