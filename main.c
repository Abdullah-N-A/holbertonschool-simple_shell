#include "shell.h"

/**
 * main - entry point for simple shell
 *
 * Return: 0 on success
 */
int main(void)
{
	char *input_line = NULL;
	size_t line_size = 0;
	ssize_t chars_read;
	char **command_args;
	int builtin_result;

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

		if (!command_args || !command_args[0])
		{
			free(command_args);
			continue;
		}

		builtin_result = check_builtin(command_args);

		if (builtin_result == -1)
		{
			free(command_args);
			free(input_line);
			exit(0);
		}
		else if (builtin_result == 0)
		{
			execute(command_args);
		}

		free(command_args);
	}

	free(input_line);
	return (0);
}
