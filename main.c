#include "shell.h"

int last_status = 0;

/**
 * cleanup_and_exit - frees memory and exits
 * @input_line: input line to free
 * @command_args: command arguments to free
 * @exit_code: exit code
 *
 * Return: void (does not return)
 */
void cleanup_and_exit(char *input_line, char **command_args, int exit_code)
{
	if (command_args)
		free(command_args);
	if (input_line)
		free(input_line);
	exit(exit_code);
}

/**
 * process_command - processes a single command
 * @command_args: parsed command arguments
 * @input_line: original input line
 *
 * Return: 0 to continue, -1 to exit
 */
int process_command(char **command_args, char *input_line)
{
	int builtin_result;

	if (!command_args || !command_args[0])
		return (0);

	builtin_result = check_builtin(command_args);

	if (builtin_result == -1)
	{
		cleanup_and_exit(input_line, command_args, last_status);
	}
	else if (builtin_result == 0)
	{
		last_status = execute(command_args);
	}

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

	while (1)
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
			process_command(command_args, input_line);

		free(command_args);
	}

	free(input_line);
	return (last_status);
}
