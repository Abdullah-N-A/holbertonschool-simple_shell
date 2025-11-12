#include "shell.h"

/* Entry point of the shell */
int main(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t nread;
    char **args;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2); /* Prompt without newline */

        nread = getline(&line, &bufsize, stdin);
        if (nread == -1)
            break; /* EOF */

        /* Remove trailing newline */
        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        args = split_line(line, " \t\r\n\a");
        if (!args || args[0] == NULL)
            continue;

        execute(args);

        free(args);
    }

    free(line);
    return 0;
}
