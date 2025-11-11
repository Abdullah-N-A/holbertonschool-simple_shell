#include "shell.h"

/**
 * find_path - Searches for the command in the directories listed in PATH.
 * @command: The command name (e.g., "ls").
 * Return: The full path to the executable, or NULL if not found.
 */
char *find_path(char *command)
{
	char *path_env, *path_copy, *dir, *full_path;
	struct stat st;

	/* If command is an absolute or relative path, no need to search PATH */
	if (strchr(command, '/') != NULL)
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		return (NULL);
	}

	/* 1. Get PATH environment variable */
	path_env = getenv("PATH");
	if (path_env == NULL)
		return (NULL);

	path_copy = strdup(path_env);
	dir = strtok(path_copy, ":");

	while (dir != NULL)
	{
		/* 2. Build the full path (dir/command) */
		full_path = build_path(dir, command);

		/* 3. Check if the file exists and is executable */
		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (full_path); /* Found! */
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL); /* Not found in any PATH directory */
}

/**
 * build_path - Concatenates a directory path and a command name.
 * @dir: The directory path.
 * @command: The command name.
 * Return: Newly allocated string for the full path (e.g., /bin/ls).
 */
char *build_path(char *dir, char *command)
{
	int len_dir, len_cmd, total_len;
	char *result;

	len_dir = strlen(dir);
	len_cmd = strlen(command);
	/* +2 for the '/' separator and the null terminator */
	total_len = len_dir + len_cmd + 2;

	result = malloc(sizeof(char) * total_len);
	if (result == NULL)
	{
		perror("malloc");
		return (NULL);
	}

	strcpy(result, dir);
	strcat(result, "/");
	strcat(result, command);

	return (result);
}
