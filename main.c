#include "shell.h"

int main(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    char **args;
    ssize_t nread;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2);
        nread = getline(&line, &bufsize, stdin);
        if (nread == -1)
        {
            free(line);
            exit(EXIT_SUCCESS);
        }
        line[nread - 1] = '\0';
        if (line[0] == '\0')
            continue;
        args = split_line(line);
        if (!args)
            continue;
        execute(args);  /* call execute but ignore returned status */
        free(args);
    }
    return (0);
}
