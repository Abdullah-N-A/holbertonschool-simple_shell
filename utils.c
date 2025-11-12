#include "shell.h"

int _strlen(char *s)
{
    int i = 0;
    while (s[i])
        i++;
    return (i);
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
    return (dest);
}

char *_strcat(char *dest, char *src)
{
    int i = 0, j = 0;
    while (dest[i])
        i++;
    while (src[j])
    {
        dest[i] = src[j];
        i++;
        j++;
    }
    dest[i] = '\0';
    return (dest);
}

char **split_line(char *line)
{
    int bufsize = 64, i = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
        exit(EXIT_FAILURE);
    token = strtok(line, " \t\r\n\a");
    while (token != NULL)
    {
        tokens[i++] = token;
        if (i >= bufsize)
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
                exit(EXIT_FAILURE);
        }
        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[i] = NULL;
    return tokens;
}
