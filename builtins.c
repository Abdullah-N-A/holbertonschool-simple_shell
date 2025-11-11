#include "shell.h"

/**
 * check_builtins - Checks if the command is a built-in and executes it.
 * @args: The array of command arguments.
 * Return: 1 if a built-in was executed, 2 if 'exit' was called, 0 otherwise.
 */
int check_builtins(char **args)
{
	if (args == NULL || args[0] == NULL)
		return (0);

	if (strcmp(args[0], "exit") == 0)
	{
		handle_exit(args); 
		return (2); /* Signal for exit */
	}
	else if (strcmp(args[0], "env") == 0)
	{
		print_env(); 
		return (1);
	}
	
	return (0); /* Not a built-in */
}

/**
 * handle_exit - Handles the 'exit' built-in command.
 * @args: The arguments array. (No args handling needed for Task 5)
 * Return: void (exits the process directly).
 */
void handle_exit(char **args)
{
	(void)args;
	free(args); /* Free the args array before final exit */
	exit(last_exit_status); /* Exit with the last command status */
}

/**
 * print_env - Prints the current environment variables.
 * Return: void
 */
void print_env(void)
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
	}
	last_exit_status = 0; /* env execution is successful */
}
