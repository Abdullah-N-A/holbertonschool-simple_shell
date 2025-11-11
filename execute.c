
#include "shell.h"

extern char **environ;

/**
 * trim_command - remove leading/trailing spaces
 * @cmd: input command
 * Return: trimmed command
 */
char *trim_command(char *cmd)
{
	char *start = cmd;
	char *end;

	while (*start == ' ')
		start++;

	end = start + _strlen(start) - 1;
	while (end > start && *end == ' ')
		end--;

	*(end + 1) = '\0';
	return (start);
}

/**
 * execute_command - run a command using fork & execve
 * @args: command and arguments
 */
void execute_command(char **args)
{
	pid_t pid;
	char *full_path;

	full_path = find_path(args[0]);
	if (!full_path)
	{
		write(STDERR_FILENO, args[0], _strlen(args[0]));
		write(STDERR_FILENO, ": command not found\n", 21);
		return;
	}

	pid = fork();
	if (pid == 0)
	{
		if (execve(full_path, args, environ) == -1)
			perror("execve");
		free(full_path);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
		wait(NULL);
	else
		perror("fork");

	free(full_path);
}
