#include "shell.h"

int _strlen(char *s)
{
    int i = 0;
    while (s[i])
        i++;
    return i;
}

char *_strcpy(char *dest, char *src)
{
    int i = 0;
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

char *_strcat(char *dest, char *src)
{
    int i = 0, j = 0;
    while (dest[i])
        i++;
    while (src[j])
        dest[i++] = src[j++];
    dest[i] = '\0';
    return dest;
}

char **split_line(char *line, const char *delim)
{
    int bufsize = 64, pos = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, delim);
    while (token != NULL)
    {
        tokens[pos++] = token;

        if (pos >= bufsize)
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delim);
    }
    tokens[pos] = NULL;
    return tokens;
}
