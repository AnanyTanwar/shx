#define _POSIX_C_SOURCE 200809L

#include "shell.h"
#include <ctype.h>

// Get formatted prompt with current directory
char *get_prompt(void)
{
    static char prompt[512];
    char cwd[PATH_MAX_LEN];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        /* Get just the last directory name */
        char *last_dir = strrchr(cwd, '/');
        if (last_dir != NULL) {
            last_dir++;
        } else {
            last_dir = cwd;
        }

        // Truncate directory name if too long
        char dir_truncated[256];
        strncpy(dir_truncated, last_dir, sizeof(dir_truncated) - 1);
        dir_truncated[sizeof(dir_truncated) - 1] = '\0';

        snprintf(prompt, sizeof(prompt), COLOR_BLUE "%s" COLOR_RESET " %s ", dir_truncated,
                 SHELL_PROMPT_CHAR);
    } else {
        snprintf(prompt, sizeof(prompt), "%s ", SHELL_PROMPT_CHAR);
    }

    return strdup(prompt);
}

// Print error message with perror if errno is set
void print_error(const char *msg)
{
    if (errno) {
        perror(msg);
    } else {
        fprintf(stderr, "%s: %s\n", SHELL_NAME, msg);
    }
}

// Trim leading and trailing whitespace from string
char *trim_whitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == 0) {
        return str;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // Write new null terminator
    end[1] = '\0';

    return str;
}

// Read a line from stdin
char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t nread;

    nread = getline(&line, &bufsize, stdin);

    if (nread == -1) {
        free(line);
        return NULL;
    }

    // Remove newline
    if (line[nread - 1] == '\n') {
        line[nread - 1] = '\0';
    }

    return line;
}
