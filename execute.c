#include "shell.h"

/**
 * execute - executes external command
 * @args: command and arguments array
 *
 * Return: exit status of command
 */
int execute(char **args)
{
	pid_t child_pid;
	int status;
	char *cmd_path;

	if (!args || !args[0])
		return (1);

	cmd_path = resolve_path(args[0]);

	if (!cmd_path)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
		return (127);
	}

	child_pid = fork();

	if (child_pid == -1)
	{
		perror("./hsh");
		free(cmd_path);
		return (1);
	}

	if (child_pid == 0)
	{
		if (execve(cmd_path, args, environ) == -1)
		{
			perror("./hsh");
			free(cmd_path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(child_pid, &status, 0);
		free(cmd_path);

		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}

	return (1);
}
