#include "main.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

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
 * run_command - execute a command with arguments, supports PATH
 * @argv_exec: NULL-terminated argv
 * @progname: shell name for error messages
 * Return: child's exit status, or 1/127 on error
 */
int run_command(char **argv_exec, const char *progname)
{
	pid_t pid;
	char *path_env, *path_copy, *dir, *full_path = NULL;
	struct stat st;
	int status = 0;

	if (!argv_exec || !argv_exec[0] || argv_exec[0][0] == '\0')
		return (0);

	/* Handle absolute or relative path directly */
	if (strchr(argv_exec[0], '/'))
	{
		if (stat(argv_exec[0], &st) == 0 && access(argv_exec[0], X_OK) == 0)
			full_path = strdup(argv_exec[0]);
		else
		{
			dprintf(STDERR_FILENO, "%s: %s: not found\n", progname, argv_exec[0]);
			return (127);
		}
	}
	else
	{
		/* Search command in PATH */
		path_env = getenv("PATH");
		if (path_env)
		{
			path_copy = strdup(path_env);
			if (!path_copy)
				return (1);

			dir = strtok(path_copy, ":");
			while (dir)
			{
				size_t len = strlen(dir) + strlen(argv_exec[0]) + 2;
				full_path = malloc(len);
				if (!full_path)
				{
					free(path_copy);
					return (1);
				}
				snprintf(full_path, len, "%s/%s", dir, argv_exec[0]);
				if (stat(full_path, &st) == 0 && access(full_path, X_OK) == 0)
				{
					free(path_copy);
					break;
				}
				free(full_path);
				full_path = NULL;
				dir = strtok(NULL, ":");
			}
			free(path_copy);
		}
		/* Command not found in PATH */
		if (!full_path)
		{
			dprintf(STDERR_FILENO, "%s: %s: not found\n", progname, argv_exec[0]);
			return (127);
		}
	}

	/* Execute command */
	pid = fork();
	if (pid == -1)
	{
		perror(progname);
		free(full_path);
		return (1);
	}

	if (pid == 0)
	{
		execve(full_path, argv_exec, environ);
		perror(progname);
		free(full_path);
		_exit(127);
	}

	free(full_path);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror(progname);
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/**
 * main - simple shell 0.3 (supports arguments and PATH)
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
