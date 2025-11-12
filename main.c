#include "shell.h"

int last_status = 0;

/**
 * process_command - processes a single command
 * @command_args: parsed command arguments
 *
 * Return: 0 to continue, -1 to exit
 */
int process_command(char **command_args)
{
	int builtin_result;

	if (!command_args || !command_args[0])
		return (0);

	builtin_result = check_builtin(command_args);

	if (builtin_result == -1)
		return (-1);
	else if (builtin_result == 0)
		last_status = execute(command_args);

	return (0);
}

/**
 * main - entry point for simple shell
 *
 * Return: last command exit status
 */
int main(void)
{
	char *input_line = NULL;
	size_t line_size = 0;
	ssize_t chars_read;
	char **command_args;
	int should_exit = 0;

	while (!should_exit)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		chars_read = getline(&input_line, &line_size, stdin);

		if (chars_read == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (input_line[chars_read - 1] == '\n')
			input_line[chars_read - 1] = '\0';

		command_args = split_line(input_line, " \t\r\n\a");

		if (command_args && command_args[0])
		{
			if (process_command(command_args) == -1)
				should_exit = 1;
		}

		free(command_args);
	}

	free(input_line);
	return (last_status);
}
