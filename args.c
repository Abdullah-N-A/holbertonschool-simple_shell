#include "main.h"

/**
 * parse_args - split a line into argv by spaces/tabs (no quotes handling)
 * @line: modifiable buffer (will be tokenized)
 * Return: NULL-terminated argv array (malloc'ed), or NULL if empty
 */
char **parse_args(char *line)
{
	char *tok;
	size_t cap = 8, argc = 0;
	char **argv_exec;

	/* skip leading spaces/tabs */
	while (*line == ' ' || *line == '\t')
		line++;

	if (*line == '\0')
		return (NULL);

	argv_exec = malloc(sizeof(char *) * cap);
	if (!argv_exec)
		return (NULL);

	tok = strtok(line, " \t");
	while (tok)
	{
		if (argc + 1 >= cap)
		{
			size_t newcap = cap * 2;
			char **tmp = realloc(argv_exec, sizeof(char *) * newcap);
			if (!tmp)
			{
				free(argv_exec);
				return (NULL);
			}
			argv_exec = tmp;
			cap = newcap;
		}
		argv_exec[argc++] = tok;
		tok = strtok(NULL, " \t");
	}
	argv_exec[argc] = NULL;

	if (argc == 0)
	{
		free(argv_exec);
		return (NULL);
	}
	return (argv_exec);
}

/**
 * free_args - free argv array holder (tokens are inside line buffer)
 */
void free_args(char **argv_exec)
{
	free(argv_exec);
}
