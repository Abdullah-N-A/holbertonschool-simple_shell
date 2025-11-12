#include "shell.h"

int builtin_exit(char **args)
{
    (void)args;
    exit(0);
}

int builtin_env(char **args)
{
    int i = 0;
    (void)args;
    while (environ[i])
    {
        printf("%s\n", environ[i]);
        i++;
    }
    return (1);
}
