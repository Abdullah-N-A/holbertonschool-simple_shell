#include "shell.h"

/**
 * print_error - Prints the required error message format.
 * @prog_name: The name of the shell program (argv[0]).
 * @command: The command that was not found.
 * * Format: ./hsh: 1: qwerty: not found
 * Note: The line number (1) is hardcoded for simplicity as per the example.
 */
void print_error(char *prog_name, char *command)
{
	/* We use write() to print to standard error (2) */
	write(STDERR_FILENO, prog_name, strlen(prog_name));
	write(STDERR_FILENO, ": 1: ", 4);
	write(STDERR_FILENO, command, strlen(command));
	write(STDERR_FILENO, ": not found\n", 12);
}
