#include "shell.h"

/**
 * _print_prompt - print prompt when stdin is a TTY
 */
static void _print_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);
}

/**
 * _sanitize_line - strip trailing newline and outer spaces/tabs
 * @s: line buffer from getline (modified in-place)
 * Return: pointer to first non-space char (or NULL)
 */
static char *_sanitize_line(char *s)
{
	size_t i = 0, end;

	if (!s)
		return (NULL);

	end = strlen(s);
	if (end && s[end - 1] == '\n')
		s[--end] = '\0';

	while (s[i] == ' ' || s[i] == '\t')
		i++;

	end = strlen(s + i);
	while (end && (s[i + end - 1] == ' ' || s[i + end - 1] == '\t'))
		end--;
	s[i + end] = '\0';

	return (s + i);
}

/**
 * _execute_oneword - exec a single word (no PATH resolution)
 * @cmd: command string (absolute/relative) to exec
 * @progname: argv[0] for perror prefix
 * Return: child's exit status, 127 on exec error
 */
static int _execute_oneword(char *cmd, char *progname)
{
	pid_t pid;
	int status = 0;
	char *argvv[2];

	if (!cmd || *cmd == '\0')
		return (0);

	argvv[0] = cmd;
	argvv[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror(progname);
		return (1);
	}
	if (pid == 0)
	{
		/* No PATH in 0.1: try exactly what user typed */
		if (execve(cmd, argvv, environ) == -1)
		{
			perror(progname);
			_exit(127);
		}
		_exit(0);
	}
	if (waitpid(pid, &status, 0) == -1)
		perror(progname);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

/**
 * run_shell - REPL: prompt, read, sanitize, exec single-word command
 * @progname: argv[0] (program name)
 * Return: last status
 */
int run_shell(char *progname)
{
	char *line = NULL, *cmd;
	size_t n = 0;
	ssize_t r;
	int status = 0;

	while (1)
	{
		_print_prompt();
		r = getline(&line, &n, stdin);
		if (r == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		cmd = _sanitize_line(line);
		if (!cmd || cmd[0] == '\0')
			continue;

		/* 0.1: treat the whole line as ONE word (no args, no PATH) */
		status = _execute_oneword(cmd, progname);
	}
	free(line);
	return (status);
}

/**
 * main - Entry point
 * @argc: argument count
 * @argv: argument vector
 * Return: exit status
 */
int main(int argc, char **argv)
{
	(void)argc;
	return (run_shell(argv[0]));
}
