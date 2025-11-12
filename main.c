#include "shell.h"

int main(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    char **args;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2);

        if (getline(&line, &bufsize, stdin) == -1)
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        line[strcspn(line, "\n")] = '\0';
        args = split_line(line, " \t\r\n\a");
        execute(args);
        free(args);
    }

    free(line);
    return 0;
}
