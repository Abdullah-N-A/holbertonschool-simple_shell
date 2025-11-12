#include "shell.h"

/* Execute command or built-in */
int execute(char **args)
{
    pid_t pid;
    int status;

    if (args[0] == NULL)
        return 1;

    if (builtin_exit(args))
        exit(0);

    if (builtin_env(args))
        return 1;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("hsh"); /* Print error if command not found */
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
