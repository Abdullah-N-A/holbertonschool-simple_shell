#include "main.h"

/**
	* main - Entry point for the simple shell
	* @argc: Argument count
	* @argv: Argument vector
	*
	* Return: Always 0 (Success)
	*/
int main(int argc, char **argv)
{
	char *line = NULL;
	char **args = NULL;
	int status = 0;

	(void)argc;
	while (1)
	{
	if (isatty(STDIN_FILENO))
	display_prompt();
	line = read_line();
	if (line == NULL)
	{
	if (isatty(STDIN_FILENO))
	write(STDOUT_FILENO, "\n", 1);
	break;
	}
	if (line[0] == '\0')
	{
	free(line);
	continue;
	}
	args = parse_line(line);
	if (args == NULL || args[0] == NULL)
	{
	free(line);
	free_array(args);
	continue;
	}
	status = execute_command(args, argv[0], line, status);
	free(line);
	free_array(args);
	}
	return (status);
}
