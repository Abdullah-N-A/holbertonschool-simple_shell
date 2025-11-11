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
	int max_args = 64; /* Initial assumption for array size */

	/* Malloc for the array of char pointers */
	args = malloc(sizeof(char *) * max_args);
	if (!args)
	{
		perror("malloc");
		return (NULL);
	}

	/* Use strtok to split the line by space, tab, newline, etc. */
	token = strtok(line, " \t\r\n\a");
	while (token != NULL)
	{
		args[i] = token;
		i++;
		/* If i reaches max_args, you'd need to realloc here for a robust shell. */
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

	/* Check if the string contains only delimiters used in strtok */
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\r' && *str != '\n')
			return (0);
		str++;
	}
	return (1);
}
