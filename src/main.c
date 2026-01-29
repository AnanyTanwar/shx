#define _POSIX_C_SOURCE 200809L
#include "shell.h"

// Global variable to track if shell should exit
volatile sig_atomic_t should_exit = 0;

int main(int argc, char *argv[], char **envp)
{
    char *line = NULL;
    char **tokens = NULL;
    Command *cmd = NULL;
    int token_count = 0;

    (void)argc;
    (void)argv;

    // Setup signal handlers
    setup_signals();

    printf("Welcome to %s v%s\n", SHELL_NAME, SHELL_VERSION);
    printf("Type 'exit' to quit\n\n");

    // Main REPL loop
    while (!should_exit) {
        // Display prompt
        char *prompt = get_prompt();
        printf("%s", prompt);
        fflush(stdout);
        free(prompt);

        // Read input
        line = read_line();
        if (line == NULL) {
            // EOF (Ctrl+D)
            printf("\n");
            break;
        }

        // Skip empty lines
        char *trimmed = trim_whitespace(line);
        if (strlen(trimmed) == 0) {
            free(line);
            continue;
        }

        // Tokenize input
        tokens = tokenize(line, &token_count);
        if (tokens == NULL || token_count == 0) {
            free(line);
            continue;
        }

        DEBUG_PRINT("Tokenized %d tokens", token_count);

        // Parse command
        cmd = parse_command(tokens, token_count);
        if (cmd == NULL) {
            // Free tokens
            for (int i = 0; i < token_count; i++) {
                free(tokens[i]);
            }
            free(tokens);
            free(line);
            continue;
        }

        // Check if builtin
        if (cmd->argc > 0 && is_builtin(cmd->args[0])) {
            execute_builtin(cmd);
        } else {
            // Execute external command
            execute_command(cmd, envp);
        }

        // Cleanup
        free_command(cmd);
        for (int i = 0; i < token_count; i++) {
            free(tokens[i]);
        }
        free(tokens);
        free(line);
    }

    printf("%s exiting.\n", SHELL_NAME);
    return EXIT_SUCCESS;
}
