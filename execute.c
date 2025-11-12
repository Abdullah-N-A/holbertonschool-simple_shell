#include "shell.h"

/**
 * execute - forks a child process to execute command
 * @args: array of command and arguments
 * Return: 1 on success, 0 if exit command
 */
int execute(char **args)
{
    pid_t pid;
    int status;

    if (args[0] == NULL)
        return 1;

    if (builtin_exit(args))
        return 0;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
            perror("hsh");
        exit(EXIT_FAILURE);
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
