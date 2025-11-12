#include "shell.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int execute(char **args)
{
    pid_t pid;
    int status;

    if (args[0] == NULL)
        return 1;

    /* Handle "exit" built-in */
    if (strcmp(args[0], "exit") == 0)
    {
        int exit_status = 0;

        if (args[1])  /* if there is an argument after exit */
            exit_status = atoi(args[1]);

        exit(exit_status);
    }

    /* Handle "env" built-in */
    if (strcmp(args[0], "env") == 0)
    {
        extern char **environ;
        char **env = environ;

        while (*env)
        {
            printf("%s\n", *env);
            env++;
        }
        return 1;
    }

    /* External commands */
    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("hsh");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("hsh");
    }
    else
    {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

