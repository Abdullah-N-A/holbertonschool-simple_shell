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

	/* Find the full path (Task 4) */
	cmd_path = find_path(args[0]);

	if (cmd_path == NULL)
	{
		/* Command not found, print error and return (Task 4 requirement) */
		print_error(prog_name, args[0]);
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
		/* Use execve to execute the command */
		if (execve(cmd_path, args, environ) == -1)
		{
			/* This should technically not be reached if find_path is correct */
			print_error(prog_name, args[0]); 
			free(cmd_path);
			/* Use _exit to safely exit the child process */
			_exit(EXIT_FAILURE); 
		}
	}
	else /* Parent process */
	{
		/* Wait for the child process to finish */
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	free(cmd_path);
	return (0);
}
