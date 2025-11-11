#include "shell.h"

/**
 * execute_command - Forks a process, finds the command, and executes it.
 * @args: The array of command and arguments.
 * @prog_name: The name of the shell program for error messages.
 * Return: 0 on success, -1 on failure.
 */
int execute_command(char **args, char *prog_name)
{
	pid_t pid;
	int status;
	char *cmd_path = NULL;

	/* Command not found scenario is handled in find_path and its return */
	cmd_path = find_path(args[0]);

	if (cmd_path == NULL)
	{
		/* Command not found, print error and set status 127 (Fix for checker) */
		print_error(prog_name, args[0]);
		last_exit_status = 127;
		return (-1);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		return (-1);
	}
	if (pid == 0) /* Child process */
	{
		if (execve(cmd_path, args, environ) == -1)
		{
			/* Should not be reached, but good practice */
			print_error(prog_name, args[0]); 
			free(cmd_path);
			_exit(EXIT_FAILURE); 
		}
	}
	else /* Parent process */
	{
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));

		if (WIFEXITED(status))
		{
			last_exit_status = WEXITSTATUS(status);
		}
	}

	free(cmd_path);
	return (0);
}
