#include "shell.h"

/**
 * run_child_process - executes command in child process
 * @cmd_path: full path to command
 * @args: command arguments
 *
 * Return: void (does not return on success)
 */
void run_child_process(char *cmd_path, char **args)
{
	if (execve(cmd_path, args, environ) == -1)
	{
		perror("./hsh");
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
}

/**
 * wait_for_child - waits for child process to complete
 * @child_pid: process ID of child
 * @cmd_path: full path to command (to free)
 *
 * Return: exit status of child process
 */
int wait_for_child(pid_t child_pid, char *cmd_path)
{
	int status;

	waitpid(child_pid, &status, 0);
	free(cmd_path);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	return (1);
}

/**
 * execute - executes external command
 * @args: command and arguments array
 *
 * Return: exit status of command
 */
int execute(char **args)
{
	pid_t child_pid;
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
		run_child_process(cmd_path, args);

	return (wait_for_child(child_pid, cmd_path));
}
