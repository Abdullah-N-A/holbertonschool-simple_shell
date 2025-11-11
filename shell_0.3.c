#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

/**
 * find_path - search for a command in the PATH
 * @cmd: command name
 * Return: full path to executable or NULL
 */
char *find_path(char *cmd)
{
    char *path_env, *path, *dir;
    static char full_path[1024];
    struct stat st;

    /* If command has '/' check directly */
    if (strchr(cmd, '/'))
    {
        if (stat(cmd, &st) == 0 && access(cmd, X_OK) == 0)
            return cmd;
        return NULL;
    }

    path_env = getenv("PATH");
    if (!path_env)
        return NULL;

    path = strdup(path_env);
    if (!path)
        return NULL;

    dir = strtok(path, ":");
    while (dir)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        if (stat(full_path, &st) == 0 && access(full_path, X_OK) == 0)
        {
            free(path);
            return full_path;
        }
        dir = strtok(NULL, ":");
    }

    free(path);
    return NULL;
}

/**
 * split_line - tokenize input string
 * @line: input line
 * Return: array of args
 */
char **split_line(char *line)
{
    char **args = NULL;
    char *token;
    size_t bufsize = 64;
    int i = 0;

    args = malloc(bufsize * sizeof(char *));
    if (!args)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n");
    while (token)
    {
        args[i++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    args[i] = NULL;
    return args;
}

/**
 * main - Simple shell 0.3
 * Return: Always 0
 */
int main(void)
{
    char *line = NULL, *path = NULL;
    size_t len = 0;
    ssize_t nread;
    pid_t pid;
    int status;
    char **args;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf(":) ");
        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break; /* Ctrl+D */
        }

        args = split_line(line);
        if (!args[0])
        {
            free(args);
            continue;
        }

        /* Check for "exit" */
        if (strcmp(args[0], "exit") == 0)
        {
            free(args);
            break;
        }

        /* Find full executable path */
        path = find_path(args[0]);
        if (!path)
        {
            fprintf(stderr, "%s: command not found\n", args[0]);
            free(args);
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(args);
            continue;
        }

        if (pid == 0)
        {
            if (execve(path, args, environ) == -1)
                perror("execve");
            exit(EXIT_FAILURE);
        }
        else
            waitpid(pid, &status, 0);

        free(args);
    }

    free(line);
    return 0;
}
