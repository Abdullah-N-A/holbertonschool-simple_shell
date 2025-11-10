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
 * Return: pointer to malloc'ed buffer (caller must free) or NULL on EOF/error
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
 * run_command - fork/exec a one-word command (no args), no PATH lookup
 * @cmd: command path or word
 * @argv0: program name to use in error messages (usually argv[0])
 * Return: exit status code of child or 0 for empty line
 */
int run_command(const char *cmd, const char *argv0)
{
	pid_t pid;
	char *argv_exec[2];

	if (cmd == NULL || *cmd == '\0')
		return (0);

	argv_exec[0] = (char *)cmd;
	argv_exec[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror(argv0);
		return (1);
	}
	if (pid == 0)
	{
		/* child */
		execve(argv_exec[0], argv_exec, environ);
		perror(argv0);
		_exit(127);
	}
	else
	{
		int status;

		if (waitpid(pid, &status, 0) == -1)
		{
			perror(argv0);
			return (1);
		}
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (1);
	}
}

/**
 * main - entry point of the simple shell 0.1
 * @argc: argument count
 * @argv: argument vector (argv[0] used in error messages)
 * Return: last command's status
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
			/* EOF: Ctrl+D or non-interactive end */
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* one-word only (no args), ignore leading spaces/tabs */
		{
			char *p = line;
			while (*p == ' ' || *p == '\t')
				p++;

			/* cut at first space/tab */
			{
				char *space = p;
				while (*space && *space != ' ' && *space != '\t')
					space++;
				*space = '\0';
			}

			if (*p != '\0')
				status = run_command(p, argv[0]);
		}

		free(line);
		line = NULL;
	}
	free(line);
	return (status);
}
