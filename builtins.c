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
		handle_exit(args); /* Handles exit (Task 5) */
		return (2); /* Signal for exit */
	}
	else if (strcmp(args[0], "env") == 0)
	{
		print_env(); /* Prints environment (Task 6) */
		return (1);
	}
	
	return (0); /* Not a built-in */
}

/**
 * handle_exit - Handles the 'exit' built-in command.
 * @args: The arguments array (for future use with exit status).
 * Return: void (exits the process directly).
 */
void handle_exit(char **args)
{
	/* Task 5 requirement: "You don’t have to handle any argument to the built-in exit" */
	(void)args;
	exit(EXIT_SUCCESS); 
}

/**
 * print_env - Prints the current environment variables.
 * Return: void
 */
void print_env(void)
{
	int i;

	/* environ is a global variable holding the environment */
	for (i = 0; environ[i] != NULL; i++)
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
	}
}
