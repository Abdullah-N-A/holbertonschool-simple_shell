#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* Handler لتجاهل Ctrl+C */
void sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n#cisfun$ ", 10);
}

/* Initialize shell structure */
shell_t *shell_init(int flags, u8 *progname, char **envp)
{
	shell_t *shell;

	(void)flags;

	shell = malloc(sizeof(shell_t));
	if (!shell)
		return NULL;

	shell->progname = progname;
	shell->envp = envp;
	shell->exit = NULL;

	return shell;
}

/* Parse input line into arguments */
char **parse_args(char *line)
{
	char **args;
	int i;
	char *token, *line_copy;

	if (!line)
		return NULL;

	line_copy = strdup(line);
	if (!line_copy)
		return NULL;

	args = malloc(64 * sizeof(char *));
	if (!args)
	{
		free(line_copy);
		return NULL;
	}

	i = 0;
	token = strtok(line_copy, " \t\n");
	while (token != NULL && i < 63)
	{
		args[i] = strdup(token);
		token = strtok(NULL, " \t\n");
		i++;
	}
	args[i] = NULL;
	free(line_copy);
	return args;
}

/* Find command in PATH */
char *find_command(char *cmd, char **envp)
{
	char *path_env = NULL, *path_dup = NULL, *path, *full_path;
	int i;

	if (!cmd || !envp)
		return NULL;

	if (strchr(cmd, '/'))
		return cmd;

	for (i = 0; envp[i]; i++)
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break;
		}
	}

	if (!path_env || path_env[0] == '\0')
		return NULL;

	path_dup = strdup(path_env);
	path = strtok(path_dup, ":");
	while (path)
	{
		full_path = malloc(strlen(path) + strlen(cmd) + 2);
		if (!full_path)
			break;
		sprintf(full_path, "%s/%s", path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_dup);
			return full_path;
		}
		free(full_path);
		path = strtok(NULL, ":");
	}

	free(path_dup);
	return NULL;
}

/* Free shell structure */
void shell_free(shell_t *shell)
{
	if (shell)
		free(shell);
}

/* Main shell loop */
void shell_runtime(shell_t *shell)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	int status;
	char *cmd_path;
	int i;
	char **args;

	/* تجاهل Ctrl+C */
	signal(SIGINT, sigint_handler);

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

		read = getline(&line, &len, stdin);
		if (read == -1)
			break;

		args = parse_args(line);
		if (!args || !args[0])
		{
			free(args);
			continue;
		}

		cmd_path = find_command(args[0], shell->envp);
		if (!cmd_path)
		{
			fprintf(stderr, "%s: %s: not found\n", shell->progname, args[0]);
			for (i = 0; args[i]; i++)
				free(args[i]);
			free(args);
			continue;
		}

		pid = fork();
		if (pid == -1)
		{
			perror((char *)shell->progname);
			for (i = 0; args[i]; i++)
				free(args[i]);
			free(args);
			if (cmd_path != args[0])
				free(cmd_path);
			continue;
		}
		else if (pid == 0)
		{
			execve(cmd_path, args, shell->envp);
			perror(args[0]);
			exit(EXIT_FAILURE);
		}
		else
		{
			waitpid(pid, &status, 0);
		}

		if (cmd_path != args[0])
			free(cmd_path);
		for (i = 0; args[i]; i++)
			free(args[i]);
		free(args);
	}

	free(line);
}
