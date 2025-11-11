#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * shell_init - initialize the shell structure
 * @flags: unused for now
 * @progname: name of the program
 * @envp: environment variables
 *
 * Return: pointer to shell_t structure
 */
shell_t *shell_init(int flags, u8 *progname, char **envp)
{
    shell_t *shell;

    (void)flags;

    shell = malloc(sizeof(shell_t));
    if (!shell)
        return NULL;

    shell->progname = progname;
    shell->envp = envp;
    shell->exit = NULL;

    return shell;
}

/**
 * parse_args - very simple parser (splits line by spaces)
 * @line: input command line
 *
 * Return: NULL-terminated array of arguments
 */
char **parse_args(char *line)
{
    char **args;
    char *token;
    char *line_copy;
    int i;

    if (!line)
        return NULL;

    line_copy = strdup(line);
    if (!line_copy)
        return NULL;

    args = malloc(64 * sizeof(char *));
    if (!args)
    {
        free(line_copy);
        return NULL;
    }

    token = strtok(line_copy, " \t\n");
    i = 0;
    while (token != NULL && i < 63)
    {
        args[i] = strdup(token);
        token = strtok(NULL, " \t\n");
        i++;
    }
    args[i] = NULL;
    free(line_copy);
    return args;
}

/**
 * find_command - searches for command in PATH
 * @cmd: command name
 * @envp: environment
 *
 * Return: full path to command if found, NULL otherwise
 */
char *find_command(char *cmd, char **envp)
{
    char *path_env;
    char *token;
    char *full_path;
    int cmd_len, token_len;

    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return strdup(cmd);
        return NULL;
    }

    path_env = NULL;
    if (envp)
    {
        int i;
        for (i = 0; envp[i]; i++)
        {
            if (strncmp(envp[i], "PATH=", 5) == 0)
            {
                path_env = envp[i] + 5;
                break;
            }
        }
    }

    if (!path_env)
        return NULL;

    token = strtok(strdup(path_env), ":");
    while (token)
    {
        cmd_len = strlen(cmd);
        token_len = strlen(token);
        full_path = malloc(token_len + 1 + cmd_len + 1);
        if (!full_path)
            return NULL;

        strcpy(full_path, token);
        full_path[token_len] = '/';
        strcpy(full_path + token_len + 1, cmd);

        if (access(full_path, X_OK) == 0)
            return full_path;

        free(full_path);
        token = strtok(NULL, ":");
    }

    return NULL;
}

/**
 * shell_runtime - main shell loop
 * @shell: pointer to shell_t
 */
void shell_runtime(shell_t *shell)
{
    char *line;
    char **args;
    size_t len;
    ssize_t read;
    pid_t pid;
    int status;
    char *cmd_path;
    int i, j;

    line = NULL;
    len = 0;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        read = getline(&line, &len, stdin);
        if (read == -1)
            break;

        args = parse_args(line);
        if (!args || !args[0])
        {
            if (args)
                free(args);
            continue;
        }

        cmd_path = find_command(args[0], shell->envp);
        if (!cmd_path)
        {
            write(STDERR_FILENO, shell->progname, strlen((char *)shell->progname));
            write(STDERR_FILENO, ": 1: ", 5);
            write(STDERR_FILENO, args[0], strlen(args[0]));
            write(STDERR_FILENO, ": not found\n", 11);

            for (j = 0; args[j]; j++)
                free(args[j]);
            free(args);
            continue;
        }

        pid = fork();
        if (pid == 0)
        {
            execve(cmd_path, args, shell->envp);
            exit(EXIT_FAILURE);
        }
        else
            waitpid(pid, &status, 0);

        for (i = 0; args[i]; i++)
            free(args[i]);
        free(args);
        free(cmd_path);
    }

    free(line);
}

/**
 * shell_free - free shell structure
 * @shell: pointer to shell_t
 */
void shell_free(shell_t *shell)
{
    if (!shell)
        return;
    free(shell);
}
