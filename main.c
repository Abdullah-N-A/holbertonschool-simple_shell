#include "main.h"

/**
 * display_prompt - print the interactive prompt
 */
void display_prompt(void)
{
	write(STDOUT_FILENO, "#cisfun$ ", 9);
}

/**
 * read_line - read one line from stdin using getline
 * Return: malloc'ed buffer (caller must free) or NULL on EOF/error
 */
char *read_line(void)
{
	char *line = NULL;
	size_t n = 0;
	ssize_t r = getline(&line, &n, stdin);

	if (r == -1)
	{
		free(line);
		return (NULL);
	}
	if (r > 0 && line[r - 1] == '\n')
		line[r - 1] = '\0';
	return (line);
}

/**
 * run_command - fork/exec a command with arguments, no PATH lookup
 * @argv_exec: NULL-terminated argv (argv_exec[0] is the path)
 * @progname: shell argv[0] to use in perror messages
 * Return: child's exit status, or 1 on error
 */
int run_command(char **argv_exec, const char *progname)
{
	pid_t pid;

	if (!argv_exec || !argv_exec[0] || argv_exec[0][0] == '\0')
		return (0);

	pid = fork();
	if (pid == -1)
	{
		perror(progname);
		return (1);
	}
	if (pid == 0)
	{
		execve(argv_exec[0], argv_exec, environ);
		perror(progname);
		_exit(127);
	}
	else
	{
		int status;

		if (waitpid(pid, &status, 0) == -1)
		{
			perror(progname);
			return (1);
		}
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (1);
	}
}

/**
 * main - simple shell 0.2 (supports arguments, no PATH)
 * @argc: argument count
 * @argv: argument vector (argv[0] used in error messages)
 * Return: last command status
 */
int main(int argc, char **argv)
{
	int status = 0;
	char *line = NULL;
	(void)argc;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			display_prompt();

		line = read_line();
		if (line == NULL)
		{
			/* EOF */
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		{
			char **argv_exec = parse_args(line);
			if (argv_exec)
			{
				status = run_command(argv_exec, argv[0]);
				free_args(argv_exec);
			}
		}

		free(line);
		line = NULL;
	}
	free(line);
	return (status);
}
