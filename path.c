#include "shell.h"

/**
 * find_path - search command in PATH
 * @cmd: command to find
 * Return: full path or NULL
 */
char *find_path(char *cmd)
{
	char *path_env, *path_copy, *dir;
	char full_path[1024];
	struct stat st;

	if (stat(cmd, &st) == 0)
		return (_strdup(cmd));

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);

	path_copy = _strdup(path_env);
	dir = strtok(path_copy, ":");
	while (dir)
	{
		_strcpy(full_path, dir);
		_strcat(full_path, "/");
		_strcat(full_path, cmd);

		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (_strdup(full_path));
		}
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
