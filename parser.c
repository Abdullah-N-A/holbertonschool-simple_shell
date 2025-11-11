#include "shell.h"

/**
 * tokenize_line - Splits the command line string into an array of arguments.
 * @line: The input string from getline.
 * Return: An array of strings (tokens), or NULL on failure.
 */
char **tokenize_line(char *line)
{
	char *token;
	char **args;
	int i = 0;
	int max_args = 64; 

	args = malloc(sizeof(char *) * max_args);
	if (!args)
	{
		perror("malloc");
		return (NULL);
	}

	token = strtok(line, " \t\r\n\a");
	while (token != NULL)
	{
		args[i] = token;
		i++;
		/* Realloc logic omitted for simplicity, stick to max_args */
		token = strtok(NULL, " \t\r\n\a");
	}
	args[i] = NULL; /* Null-terminate the array for execve */
	return (args);
}

/**
 * check_for_whitespace - Checks if a string contains only whitespace or is empty.
 * @str: The input string.
 * Return: 1 if empty/whitespace only, 0 otherwise.
 */
int check_for_whitespace(char *str)
{
	if (str == NULL || *str == '\0')
		return (1);

	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\r' && *str != '\n')
			return (0);
		str++;
	}
	return (1);
}
