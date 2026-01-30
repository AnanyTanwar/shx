#define _POSIX_C_SOURCE 200809L
#include "shell.h"

// Execute external command
int execute_command(Command *cmd, char **envp)
{
    if (cmd == NULL || cmd->argc == 0) {
        return -1;
    }

    (void)envp; // Unused for now

    pid_t pid;
    int status;
    int saved_stdin = -1, saved_stdout = -1;

    // Setup redirections if needed
    if (cmd->input_file || cmd->output_file) {
        saved_stdin = dup(STDIN_FILENO);
        saved_stdout = dup(STDOUT_FILENO);

        if (setup_redirections(cmd) != 0) {
            if (saved_stdin >= 0)
                close(saved_stdin);
            if (saved_stdout >= 0)
                close(saved_stdout);
            return -1;
        }
    }

    pid = fork();

    if (pid < 0) {
        // Fork failed
        print_error("fork");
        restore_redirections(saved_stdin, saved_stdout);
        return -1;
    } else if (pid == 0) {
        // Child process

        // Reset signal handlers to default
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        // Execute command
        execvp(cmd->args[0], cmd->args);

        // If we get here, exec failed
        fprintf(stderr, "%s: command not found: %s\n", SHELL_NAME, cmd->args[0]);
        exit(127);
    } else {
        // Parent process

        if (cmd->background) {
            // Background process - don't wait
            printf("[%d] %d\n", 1, pid);
        } else {
            // Foreground process - wait for completion
            do {
                if (waitpid(pid, &status, WUNTRACED) == -1) {
                    print_error("waitpid");
                    break;
                }
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

        // Restore redirections
        restore_redirections(saved_stdin, saved_stdout);
    }

    return 0;
}

// Execute pipeline of commands (for future pipe support)
int execute_pipeline(Command **commands, int cmd_count, char **envp)
{
    // TODO: Implement pipeline execution with pipes
    (void)commands;
    (void)cmd_count;
    (void)envp;

    fprintf(stderr, "Pipelines not yet implemented\n");
    return -1;
}
