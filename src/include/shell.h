#ifndef SHELL_H
#define SHELL_H

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"

#define MAX_ARGS 64
#define MAX_TOKENS 128

// Command structure
typedef struct {
    char **args;       // Array of arguments
    int argc;          // Number of arguments
    char *input_file;  // Input redirection file
    char *output_file; // Output redirection file
    int append;        // Append mode for outputs
    int background;    // Run in background
} Command;

// Global variable for exit control
extern volatile sig_atomic_t should_exit;

// Parser functions
char *read_line(void);
char **tokenize(char *line, int *token_count);
Command *parse_command(char **tokens, int token_count);
void free_command(Command *cmd);

// Executor functions
int execute_command(Command *cmd, char **envp);
int execute_pipeline(Command **commands, int cmd_count, char **envp);

// Builtin functions
int is_builtin(char *cmd);
int execute_builtin(Command *cmd);
int builtin_cd(char **args);
int builtin_exit(char **args);
int builtin_pwd(char **args);
int builtin_echo(char **args);
int builtin_export(char **args);
int builtin_env(char **args);

// Redirection functions
int setup_redirections(Command *cmd);
void restore_redirections(int saved_stdin, int saved_stdout);

// Signal handling
void setup_signals(void);
void handle_sigint(int sig);
void handle_sigchld(int sig);

// Utility functions
char *get_prompt(void);
void print_error(const char *msg);
char *trim_whitespace(char *str);

// Environment functions
char *get_env_var(const char *name);
int set_env_var(const char *name, const char *value);

#endif // SHELL_H
