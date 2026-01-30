#define _POSIX_C_SOURCE 200809L
#include "shell.h"

// Setup input and output redirections
int setup_redirections(Command *cmd)
{
    if (cmd == NULL) {
        return -1;
    }

    // Handle input redirection
    if (cmd->input_file) {
        int fd = open(cmd->input_file, O_RDONLY);
        if (fd < 0) {
            print_error(cmd->input_file);
            return -1;
        }

        if (dup2(fd, STDIN_FILENO) < 0) {
            print_error("dup2");
            close(fd);
            return -1;
        }
        close(fd);
    }

    // Handle output redirection
    if (cmd->output_file) {
        int flags = O_WRONLY | O_CREAT;
        flags |= cmd->append ? O_APPEND : O_TRUNC;

        int fd = open(cmd->output_file, flags, 0644);
        if (fd < 0) {
            print_error(cmd->output_file);
            return -1;
        }

        if (dup2(fd, STDOUT_FILENO) < 0) {
            print_error("dup2");
            close(fd);
            return -1;
        }
        close(fd);
    }

    return 0;
}

// Restore stdin and stdout
void restore_redirections(int saved_stdin, int saved_stdout)
{
    if (saved_stdin >= 0) {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
    }

    if (saved_stdout >= 0) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
}
