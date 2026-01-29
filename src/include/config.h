#ifndef CONFIG_H
#define CONFIG_H

#include <limits.h>

// Shell configuration
#define SHELL_NAME "shx"
#define SHELL_VERSION "0.1.0"
#define SHELL_PROMPT_CHAR "$"

// Buffer sizes
#define BUFFER_SIZE 1024
#define PATH_MAX_LEN 4096

// Color codes for prompt
#define COLOR_RESET "\033[0m"
#define COLOR_BLUE "\033[34m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"

// Debug mode
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...)                                                                      \
    fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)                                                                      \
    do {                                                                                           \
    } while (0)
#endif

#endif // CONFIG_H
