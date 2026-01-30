#define _POSIX_C_SOURCE 200809L
#include "shell.h"

// Handle SIGINT (Ctrl+C)
void handle_sigint(int sig)
{
    (void)sig;

    // Print newline and show prompt again
    printf("\n");
    fflush(stdout);

    // Don't exit the shell, just cancel current line
}

// Handle SIGCHLD (child process terminated)
void handle_sigchld(int sig)
{
    (void)sig;

    // Reap zombie processes
    int saved_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        // Continue reaping
    }
    errno = saved_errno;
}

// Setup signal handlers
void setup_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_chld;

    // Setup SIGINT handler
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        print_error("sigaction SIGINT");
    }

    // Setup SIGCHLD handler
    sa_chld.sa_handler = handle_sigchld;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        print_error("sigaction SIGCHLD");
    }

    // Ignore SIGQUIT
    signal(SIGQUIT, SIG_IGN);
}
