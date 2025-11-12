#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

/* Function to get PATH from environ */
char *get_path(void)
{
    int i = 0;

    while (environ[i])
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
            return environ[i] + 5;
        i++;
    }
    return NULL;
}

/* Split string by delimiter */
char **split_line(char *line, const char *delim)
{
    int bufsize = 64, pos = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, delim);
    while (token != NULL)
    {
        tokens[pos++] = token;

        if (pos >= bufsize)
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delim);
    }
    tokens[pos] = NULL;
    return tokens;
}

/* Execute command */
void execute(char **args)
{
    pid_t pid;
    int status;
    char *path_env = get_path();
    char *path_copy, *dir;
    char cmd_path[1024];

    if (args[0] == NULL)
        return;

    /* built-in exit */
    if (strcmp(args[0], "exit") == 0)
        exit(0);

    pid = fork();
    if (pid == 0) /* Child */
    {
        /* If command contains '/' try to execute directly */
        if (strchr(args[0], '/'))
        {
            execve(args[0], args, environ);
            perror(args[0]);
            exit(EXIT_FAILURE);
        }

        if (path_env)
        {
            path_copy = strdup(path_env);
            dir = strtok(path_copy, ":");
            while (dir)
            {
                snprintf(cmd_path, sizeof(cmd_path), "%s/%s", dir, args[0]);
                execve(cmd_path, args, environ);
                dir = strtok(NULL, ":");
            }
            free(path_copy);
        }

        /* Command not found */
        fprintf(stderr, "%s: 1: %s: not found\n", args[0], args[0]);
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) /* Parent */
    {
        waitpid(pid, &status, 0);
    }
    else
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    char **args;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2);

        if (getline(&line, &bufsize, stdin) == -1)
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        line[strcspn(line, "\n")] = '\0'; /* remove newline */
        args = split_line(line, " \t\r\n\a");
        execute(args);
        free(args);
    }

    free(line);
    return 0;
}
