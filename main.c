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
 * parse_args - split a line into argv by spaces/tabs (no quotes handling)
 * @line: modifiable buffer (will be tokenized)
 * Return: NULL-terminated argv array (malloc'ed), or NULL if empty
 */
char **parse_args(char *line)
{
	char *tok;
	size_t cap = 8, argc = 0;
	char **argv_exec;

	/* skip leading spaces/tabs */
	while (*line == ' ' || *line == '\t')
		line++;

	if (*line == '\0')
		return (NULL);

	argv_exec = malloc(sizeof(char *) * cap);
	if (!argv_exec)
		return (NULL);

	tok = strtok(line, " \t");
	while (tok)
	{
		if (argc + 1 >= cap)
		{
			size_t newcap = cap * 2;
			char **tmp = realloc(argv_exec, sizeof(char *) * newcap);
			if (!tmp)
			{
				free(argv_exec);
				return (NULL);
			}
			argv_exec = tmp;
			cap = newcap;
		}
		argv_exec[argc++] = tok;
		tok = strtok(NULL, " \t");
	}
	argv_exec[argc] = NULL;

	if (argc == 0)
	{
		free(argv_exec);
		return (NULL);
	}
	return (argv_exec);
}

/**
 * free_args - free argv array holder (tokens are inside line buffer)
 */
void free_args(char **argv_exec)
{
	free(argv_exec);
}

/**
 * run_command - fork/exec a command with arguments, no PATH lookup
 * @argv_exec: NULL-terminated argv (argv_exec[0] is the path, e.g. ./hbtn_ls)
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
		/* child */
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
 * main - entry point of the simple shell (0.1 + args support)
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

		/* tokenize line into argv and exec (no PATH resolution) */
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
