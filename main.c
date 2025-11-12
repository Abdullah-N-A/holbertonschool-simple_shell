#include "shell.h"

/**
 * main - simple shell main loop
 * Return: 0 on success, 1 on failure
 */
int main(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t nread;
    char **args;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, ":) ", 3);

        nread = getline(&line, &bufsize, stdin);
        if (nread == -1)
            break;

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
