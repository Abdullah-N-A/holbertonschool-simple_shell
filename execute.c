#include "shell.h"

int execute(char **args)
{
    pid_t pid;
    int status;
    char *cmd_path;

    if (args[0] == NULL)
        return 1;

    if (strcmp(args[0], "exit") == 0)
        exit(0);

    pid = fork();
    if (pid == 0)
    {
        if (strchr(args[0], '/'))
        {
            execve(args[0], args, environ);
            perror(args[0]);
            exit(EXIT_FAILURE);
        }

        cmd_path = find_path(args[0]);
        if (cmd_path)
        {
            execve(cmd_path, args, environ);
            perror(args[0]);
            exit(EXIT_FAILURE);
        }

        fprintf(stderr, "%s: not found\n", args[0]);
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
    }
    else
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 1;
}
