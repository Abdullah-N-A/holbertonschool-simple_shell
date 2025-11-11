#include "shell.h"

/**
 * parse_command - split command by spaces
 * @line: input line
 * Return: array of arguments
 */
char **parse_command(char *line)
{
	char **args;
	char *token;
	int i = 0;

	args = malloc(64 * sizeof(char *));
	if (!args)
		return (NULL);

	token = strtok(line, " ");
	while (token != NULL)
	{
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	return (args);
}
