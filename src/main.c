#define _POSIX_C_SOURCE 200809L
#include "shell.h"

// Global variable to track if shell should exit
volatile sig_atomic_t should_exit = 0;

int main(int argc, char *argv[], char **envp)
{
    char *line = NULL;
    char **tokens = NULL;
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

        // Parse pipeline (handles both single commands and pipes)
        Pipeline *pipeline = parse_pipeline(tokens, token_count);
        if (pipeline == NULL) {
            // Free tokens
            for (int i = 0; i < token_count; i++) {
                free(tokens[i]);
            }
            free(tokens);
            free(line);
            continue;
        }

        // Execute pipeline
        if (pipeline->count == 1 && is_builtin(pipeline->commands[0]->args[0])) {
            // Single builtin command
            execute_builtin(pipeline->commands[0]);
        } else {
            // Pipeline or external command
            execute_pipeline(pipeline, envp);
        }

        // Cleanup
        free_pipeline(pipeline);
        for (int i = 0; i < token_count; i++) {
            free(tokens[i]);
        }
        free(tokens);
        free(line);
    }

    printf("%s exiting.\n", SHELL_NAME);
    return EXIT_SUCCESS;
}
