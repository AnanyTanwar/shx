#define _POSIX_C_SOURCE 200809L
#include "shell.h"

extern volatile sig_atomic_t should_exit;

// Check if command is a builtin
int is_builtin(char *cmd)
{
    if (cmd == NULL) {
        return 0;
    }

    return (strcmp(cmd, "cd") == 0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "pwd") == 0 ||
            strcmp(cmd, "echo") == 0 || strcmp(cmd, "export") == 0 || strcmp(cmd, "env") == 0);
}

// Execute builtin command
int execute_builtin(Command *cmd)
{
    if (cmd == NULL || cmd->argc == 0) {
        return -1;
    }

    char *name = cmd->args[0];

    if (strcmp(name, "cd") == 0) {
        return builtin_cd(cmd->args);
    } else if (strcmp(name, "exit") == 0) {
        return builtin_exit(cmd->args);
    } else if (strcmp(name, "pwd") == 0) {
        return builtin_pwd(cmd->args);
    } else if (strcmp(name, "echo") == 0) {
        return builtin_echo(cmd->args);
    } else if (strcmp(name, "export") == 0) {
        return builtin_export(cmd->args);
    } else if (strcmp(name, "env") == 0) {
        return builtin_env(cmd->args);
    }

    return -1;
}

// cd - change directory
int builtin_cd(char **args)
{
    char *path;

    if (args[1] == NULL) {
        // No argument, go to HOME
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return -1;
        }
    } else {
        path = args[1];
    }

    if (chdir(path) != 0) {
        print_error("cd");
        return -1;
    }

    return 0;
}

// exit - exit the shell
int builtin_exit(char **args)
{
    int exit_code = 0;

    if (args[1] != NULL) {
        exit_code = atoi(args[1]);
    }

    should_exit = 1;
    return exit_code;
}

// pwd - print working directory
int builtin_pwd(char **args)
{
    char cwd[PATH_MAX_LEN];

    (void)args; // Unused

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
        return 0;
    } else {
        print_error("pwd");
        return -1;
    }
}

// echo - print arguments
int builtin_echo(char **args)
{
    for (int i = 1; args[i] != NULL; i++) {
        printf("%s", args[i]);
        if (args[i + 1] != NULL) {
            printf(" ");
        }
    }
    printf("\n");
    return 0;
}

// export - set environment variable
int builtin_export(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "export: usage: export VAR=value\n");
        return -1;
    }

    // Parse VAR=value
    char *equals = strchr(args[1], '=');
    if (equals == NULL) {
        fprintf(stderr, "export: usage: export VAR=value\n");
        return -1;
    }

    *equals = '\0';
    char *name = args[1];
    char *value = equals + 1;

    if (setenv(name, value, 1) != 0) {
        print_error("export");
        return -1;
    }

    return 0;
}

// env - print environment variables
int builtin_env(char **args)
{
    extern char **environ;

    (void)args; // Unused

    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }

    return 0;
}
