#include "shell.h"

/**
 * builtin_exit - handles exit command
 * @args: command arguments
 *
 * Return: -1 to signal program exit
 */
int builtin_exit(char **args)
{
	int exit_code = 0;

	if (args[1])
	{
		if (!validate_number(args[1]))
		{
			fprintf(stderr, "./hsh: 1: exit: Illegal number: %s\n", args[1]);
			return (0);
		}

		exit_code = str_to_int(args[1]);
		exit_code = exit_code % 256;
		if (exit_code < 0)
			exit_code = 256 + exit_code;
		last_status = exit_code;
	}

	return (-1);
}

/**
 * builtin_env - prints all environment variables
 * @args: command arguments (unused)
 *
 * Return: 1 after execution
 */
int builtin_env(char **args)
{
	int i;

	(void)args;

	if (!environ)
		return (1);

	for (i = 0; environ[i]; i++)
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
	}

	return (1);
}

/**
 * check_builtin - determines if command is built-in
 * @args: command arguments
 *
 * Return: -1 for exit, 1 if builtin executed, 0 if not builtin
 */
int check_builtin(char **args)
{
	if (!args || !args[0])
		return (0);

	if (strcmp(args[0], "exit") == 0)
		return (builtin_exit(args));

	if (strcmp(args[0], "env") == 0)
		return (builtin_env(args));

	return (0);
}
