#include "shell.h"

extern char **environ;  /* Add this line */

/* Exit built-in */
int builtin_exit(char **args)
{
    if (strcmp(args[0], "exit") == 0)
        return 1;
    return 0;
}

/* Env built-in */
int builtin_env(char **args)
{
    int i;

    if (strcmp(args[0], "env") == 0)
    {
        for (i = 0; environ[i]; i++)
        {
            write(STDOUT_FILENO, environ[i], strlen(environ[i]));
            write(STDOUT_FILENO, "\n", 1);
        }
        return 1;
    }
    return 0;
}
