#include "shell.h"

int execute(char **args)
{
    pid_t pid;
    int status;
    char *cmd_path;

    if (!args[0])
        return (1);

    if (strcmp(args[0], "exit") == 0)
        return (builtin_exit(args));
    if (strcmp(args[0], "env") == 0)
        return (builtin_env(args));

    if (access(args[0], F_OK) == 0)
        cmd_path = args[0];
    else
        cmd_path = find_path(args[0]);

    if (!cmd_path)
    {
        fprintf(stderr, "%s: 1: %s: not found\n", args[0], args[0]);
        return (1);
    }

    pid = fork();
    if (pid == 0)
        execve(cmd_path, args, environ);
    else if (pid < 0)
        perror("fork");
    else
    {
        waitpid(pid, &status, 0);
    }
    return (1);
}
