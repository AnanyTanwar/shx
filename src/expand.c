#define _POSIX_C_SOURCE 200809L
#include "shell.h"
#include <ctype.h>

// Check if character is valid in variable name
static int is_var_char(char c)
{
    return isalnum(c) || c == '_';
}

// Expand a single variable like $HOME or ${HOME}
static char *expand_variable(const char *var_name)
{
    if (!var_name || strlen(var_name) == 0) {
        return strdup("");
    }

    // Special variables
    if (strcmp(var_name, "?") == 0) {
        // Last exit status (TODO: implement)
        return strdup("0");
    }
    if (strcmp(var_name, "$") == 0) {
        // Shell PID
        char pid_str[32];
        snprintf(pid_str, sizeof(pid_str), "%d", getpid());
        return strdup(pid_str);
    }

    // Get from environment
    char *value = getenv(var_name);
    if (value) {
        return strdup(value);
    }

    // Variable not found - return empty string
    return strdup("");
}

// Expand all variables in a string
char *expand_env_vars(const char *str)
{
    if (!str) {
        return NULL;
    }

    size_t result_size = 256;
    char *result = malloc(result_size);
    if (!result) {
        return NULL;
    }

    size_t result_len = 0;
    const char *p = str;

    while (*p) {
        if (*p == '$') {
            p++; // Skip $

            char var_name[256] = {0};
            int var_len = 0;

            // Handle ${VAR} syntax
            if (*p == '{') {
                p++; // Skip {
                while (*p && *p != '}' && var_len < 255) {
                    var_name[var_len++] = *p++;
                }
                if (*p == '}') {
                    p++; // Skip }
                }
            }
            // Handle $VAR syntax
            else {
                while (*p && is_var_char(*p) && var_len < 255) {
                    var_name[var_len++] = *p++;
                }
            }

            var_name[var_len] = '\0';

            // Expand the variable
            char *value = expand_variable(var_name);
            size_t value_len = strlen(value);

            // Ensure buffer is big enough
            while (result_len + value_len + 1 > result_size) {
                result_size *= 2;
                char *new_result = realloc(result, result_size);
                if (!new_result) {
                    free(result);
                    free(value);
                    return NULL;
                }
                result = new_result;
            }

            // Copy expanded value
            strcpy(result + result_len, value);
            result_len += value_len;
            free(value);
        } else {
            // Regular character
            if (result_len + 2 > result_size) {
                result_size *= 2;
                char *new_result = realloc(result, result_size);
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[result_len++] = *p++;
        }
    }

    result[result_len] = '\0';
    return result;
}

// Expand variables in all command arguments
void expand_command_vars(Command *cmd)
{
    if (!cmd || !cmd->args) {
        return;
    }

    for (int i = 0; i < cmd->argc; i++) {
        char *expanded = expand_env_vars(cmd->args[i]);
        if (expanded) {
            free(cmd->args[i]);
            cmd->args[i] = expanded;
        }
    }

    // Also expand in redirections
    if (cmd->input_file) {
        char *expanded = expand_env_vars(cmd->input_file);
        if (expanded) {
            free(cmd->input_file);
            cmd->input_file = expanded;
        }
    }

    if (cmd->output_file) {
        char *expanded = expand_env_vars(cmd->output_file);
        if (expanded) {
            free(cmd->output_file);
            cmd->output_file = expanded;
        }
    }
}
