#define _POSIX_C_SOURCE 200809L
#include "shell.h"

// Get environment variable value
char *get_env_var(const char *name)
{
    if (name == NULL) {
        return NULL;
    }

    return getenv(name);
}

// Set environment variable
int set_env_var(const char *name, const char *value)
{
    if (name == NULL || value == NULL) {
        return -1;
    }

    return setenv(name, value, 1);
}
