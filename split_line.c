#include "shell.h"

/* Function to split the line into tokens (commands and arguments) */
char **split_line(char *line, const char *delim)
{
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
        exit(EXIT_FAILURE);

    token = strtok(line, delim);
    while (token != NULL)
    {
        tokens[position++] = token;
        if (position >= bufsize)
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
                exit(EXIT_FAILURE);
        }
        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;
    return tokens;
}
