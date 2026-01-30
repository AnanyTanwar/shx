#define _POSIX_C_SOURCE 200809L
#include "shell.h"
#include <ctype.h>

// Tokenize input line into array of strings
char **tokenize(char *line, int *token_count)
{
    char **tokens = malloc(MAX_TOKENS * sizeof(char *));
    if (tokens == NULL) {
        print_error("malloc failed");
        return NULL;
    }

    int count = 0;
    char *token;
    char *saveptr;

    // Split by whitespace
    token = strtok_r(line, " \t\n", &saveptr);
    while (token != NULL && count < MAX_TOKENS) {
        tokens[count] = strdup(token);
        if (tokens[count] == NULL) {
            // Cleanup on error
            for (int i = 0; i < count; i++) {
                free(tokens[i]);
            }
            free(tokens);
            return NULL;
        }
        count++;
        token = strtok_r(NULL, " \t\n", &saveptr);
    }

    *token_count = count;
    return tokens;
}

// Parse tokens into Command structure
Command *parse_command(char **tokens, int token_count)
{
    if (token_count == 0) {
        return NULL;
    }

    Command *cmd = calloc(1, sizeof(Command));
    if (cmd == NULL) {
        print_error("malloc failed");
        return NULL;
    }

    cmd->args = malloc((MAX_ARGS + 1) * sizeof(char *));
    if (cmd->args == NULL) {
        free(cmd);
        return NULL;
    }

    int arg_idx = 0;

    for (int i = 0; i < token_count; i++) {
        if (strcmp(tokens[i], ">") == 0) {
            // Output redirection
            if (i + 1 < token_count) {
                cmd->output_file = strdup(tokens[++i]);
                cmd->append = 0;
            } else {
                fprintf(stderr, "syntax error: expected filename after '>'\n");
                free_command(cmd);
                return NULL;
            }
        } else if (strcmp(tokens[i], ">>") == 0) {
            // Append redirection
            if (i + 1 < token_count) {
                cmd->output_file = strdup(tokens[++i]);
                cmd->append = 1;
            } else {
                fprintf(stderr, "syntax error: expected filename after '>>'\n");
                free_command(cmd);
                return NULL;
            }
        } else if (strcmp(tokens[i], "<") == 0) {
            // Input redirection
            if (i + 1 < token_count) {
                cmd->input_file = strdup(tokens[++i]);
            } else {
                fprintf(stderr, "syntax error: expected filename after '<'\n");
                free_command(cmd);
                return NULL;
            }
        } else if (strcmp(tokens[i], "&") == 0) {
            // Background execution
            cmd->background = 1;
        } else {
            // Regular argument
            if (arg_idx < MAX_ARGS) {
                cmd->args[arg_idx++] = strdup(tokens[i]);
            }
        }
    }

    cmd->args[arg_idx] = NULL; /* Null terminate */
    cmd->argc = arg_idx;

    return cmd;
}

// Free command structure
void free_command(Command *cmd)
{
    if (cmd == NULL) {
        return;
    }

    if (cmd->args != NULL) {
        for (int i = 0; cmd->args[i] != NULL; i++) {
            free(cmd->args[i]);
        }
        free(cmd->args);
    }

    free(cmd->input_file);
    free(cmd->output_file);
    free(cmd);
}
