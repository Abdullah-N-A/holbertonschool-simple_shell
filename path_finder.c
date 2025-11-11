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

	/* If command is an absolute or relative path */
	if (strchr(command, '/') != NULL)
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		return (NULL);
	}

	path_env = getenv("PATH");
	
	/* Handles empty PATH and NULL PATH correctly */
	if (path_env == NULL || *path_env == '\0')
		return (NULL);

	path_copy = strdup(path_env);
	dir = strtok(path_copy, ":");

	while (dir != NULL)
	{
		full_path = build_path(dir, command);

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
	int total_len;
	char *result;

	/* +2 for the '/' separator and the null terminator */
	total_len = strlen(dir) + strlen(command) + 2;

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
