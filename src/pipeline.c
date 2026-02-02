#define _POSIX_C_SOURCE 200809L
#include "shell.h"

// Parse tokens into pipeline (split by |)
Pipeline *parse_pipeline(char **tokens, int token_count)
{
    if (token_count == 0) {
        return NULL;
    }

    Pipeline *pl = malloc(sizeof(Pipeline));
    if (!pl) {
        print_error("malloc failed");
        return NULL;
    }

    // Count number of pipes to know how many commands
    int pipe_count = 0;
    for (int i = 0; i < token_count; i++) {
        if (strcmp(tokens[i], "|") == 0) {
            pipe_count++;
        }
    }

    pl->count = pipe_count + 1; // commands = pipes + 1
    pl->commands = malloc(pl->count * sizeof(Command *));
    if (!pl->commands) {
        free(pl);
        return NULL;
    }

    // Split tokens by | and create commands
    int cmd_idx = 0;
    int start = 0;

    for (int i = 0; i <= token_count; i++) {
        if (i == token_count || strcmp(tokens[i], "|") == 0) {
            // Found a pipe or end of tokens
            int cmd_len = i - start;

            if (cmd_len == 0) {
                fprintf(stderr, "syntax error: empty command in pipeline\n");
                free_pipeline(pl);
                return NULL;
            }

            // Create command from this segment
            pl->commands[cmd_idx] = parse_command(&tokens[start], cmd_len);
            if (!pl->commands[cmd_idx]) {
                // Cleanup already parsed commands
                for (int j = 0; j < cmd_idx; j++) {
                    free_command(pl->commands[j]);
                }
                free(pl->commands);
                free(pl);
                return NULL;
            }

            cmd_idx++;
            start = i + 1; // Skip the | token
        }
    }


    // Expand environment variables in all commands
    for (int i = 0; i < pl->count; i++) {
        if (pl->commands[i]) {
            expand_command_vars(pl->commands[i]);
        }
    }

    return pl;
}

// Free pipeline structure
void free_pipeline(Pipeline *pl)
{
    if (!pl) {
        return;
    }

    if (pl->commands) {
        for (int i = 0; i < pl->count; i++) {
            free_command(pl->commands[i]);
        }
        free(pl->commands);
    }

    free(pl);
}

// Execute pipeline with pipes
int execute_pipeline(Pipeline *pl, char **envp)
{
    if (!pl || pl->count == 0) {
        return -1;
    }

    // Single command - no pipes needed
    if (pl->count == 1) {
        return execute_command(pl->commands[0], envp);
    }

    (void)envp; // Will use later

    int pipefds[2 * (pl->count - 1)]; // 2 fds per pipe
    pid_t pids[pl->count];

    // Create all pipes
    for (int i = 0; i < pl->count - 1; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            print_error("pipe");
            return -1;
        }
    }

    // Fork and execute each command
    for (int i = 0; i < pl->count; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            print_error("fork");
            return -1;
        }

        if (pids[i] == 0) {
            // Child process

            // If not first command, get input from previous pipe
            if (i > 0) {
                if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) < 0) {
                    print_error("dup2");
                    exit(1);
                }
            }

            // If not last command, send output to next pipe
            if (i < pl->count - 1) {
                if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0) {
                    print_error("dup2");
                    exit(1);
                }
            }

            // Close all pipe fds in child
            for (int j = 0; j < 2 * (pl->count - 1); j++) {
                close(pipefds[j]);
            }

            // Execute the command
            Command *cmd = pl->commands[i];

            // Handle redirections if any
            if (cmd->input_file || cmd->output_file) {
                setup_redirections(cmd);
            }

            // Check if builtin
            if (is_builtin(cmd->args[0])) {
                exit(execute_builtin(cmd));
            } else {
                execvp(cmd->args[0], cmd->args);
                fprintf(stderr, "%s: command not found: %s\n", SHELL_NAME, cmd->args[0]);
                exit(127);
            }
        }
    }

    // Parent: close all pipes
    for (int i = 0; i < 2 * (pl->count - 1); i++) {
        close(pipefds[i]);
    }

    // Wait for all children
    for (int i = 0; i < pl->count; i++) {
        int status;
        waitpid(pids[i], &status, 0);
    }

    return 0;
}
