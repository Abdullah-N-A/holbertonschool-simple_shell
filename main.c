#include "shell.h"

/* Initialize the global exit status variable */
int last_exit_status = 0;

/**
 * main - Entry point for the simple shell program.
 * @ac: The argument count. (Unused)
 * @av: The argument vector (Used for error reporting argv[0]).
 * Return: The last exit status.
 */
int main(int ac, char **av)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_bytes;
	(void)ac; /* To avoid 'unused parameter' warning */

	while (1) /* Main shell loop */
	{
		if (isatty(STDIN_FILENO))
			print_prompt(); 

		read_bytes = getline(&line, &len, stdin);

		if (read_bytes == -1) /* Handle EOF (Ctrl+D) or read error */
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1); 
			free(line);
			return (last_exit_status); /* Exit with the last known status (Fix for 127) */
		}

		if (read_bytes > 0 && line[read_bytes - 1] == '\n')
			line[read_bytes - 1] = '\0';
		
		if (check_for_whitespace(line))
			continue;

		if (handle_command(line, av[0]) == -1)
		{
			; /* Error handled inside sub-functions */
		}
	}
	/* Return statement added inside EOF check */
}

/**
 * handle_command - Parses and executes the command line.
 * @line: The raw input string from the user.
 * @prog_name: The name of the shell program (argv[0]) for error messages.
 * Return: 0 on success, -1 on failure.
 */
int handle_command(char *line, char *prog_name)
{
	char **args;
	int status = 0;

	args = tokenize_line(line); 
	if (args == NULL || args[0] == NULL)
	{
		if (args != NULL)
			free(args);
		return (-1);
	}

	/* Check for Built-ins (exit, env) */
	status = check_builtins(args);
	if (status == 1) /* Built-in executed (e.g., env) */
	{
		free(args);
		return (0);
	}
	else if (status == 2) /* Built-in signals exit (e.g., exit) */
	{
		free(args);
		/* handle_exit already calls exit() */
		return (0); 
	}

	/* Execute external command */
	execute_command(args, prog_name);

	/* Clean up */
	free(args);
	return (0);
}

/**
 * print_prompt - Prints the shell prompt in interactive mode.
 * Return: void
 */
void print_prompt(void)
{
	write(STDOUT_FILENO, "($) ", 4);
}
