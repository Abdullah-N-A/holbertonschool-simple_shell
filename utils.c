#include "shell.h"

/**
 * find_env_match - finds matching environment variable
 * @name: variable name
 * @len: length of name
 *
 * Return: pointer to value or NULL
 */
static char *find_env_match(char *name, int len)
{
	int i, j;

	for (i = 0; environ[i]; i++)
	{
		for (j = 0; j < len && environ[i][j] == name[j]; j++)
			;
		if (j == len && environ[i][j] == '=')
			return (&environ[i][j + 1]);
	}
	return (NULL);
}

/**
 * get_env_value - retrieves environment variable value
 * @name: name of environment variable
 *
 * Return: pointer to value or NULL
 */
char *get_env_value(char *name)
{
	int len;

	if (!name || !environ)
		return (NULL);

	len = strlen(name);
	return (find_env_match(name, len));
}

/**
 * build_full_path - constructs full path from directory and command
 * @dir: directory path
 * @command: command name
 *
 * Return: full path string or NULL
 */
static char *build_full_path(char *dir, char *command)
{
	char *full_path;
	int dir_len, cmd_len;

	dir_len = strlen(dir);
	cmd_len = strlen(command);
	full_path = malloc(dir_len + cmd_len + 2);

	if (!full_path)
		return (NULL);

	strcpy(full_path, dir);
	full_path[dir_len] = '/';
	strcpy(full_path + dir_len + 1, command);

	return (full_path);
}

/**
 * search_in_path - searches for command in PATH directories
 * @path_dup: duplicate of PATH variable
 * @command: command to find
 *
 * Return: full path or NULL
 */
static char *search_in_path(char *path_dup, char *command)
{
	char *token, *full_path;
	struct stat file_stat;

	token = strtok(path_dup, ":");
	while (token)
	{
		full_path = build_full_path(token, command);
		if (!full_path)
		{
			free(path_dup);
			return (NULL);
		}

		if (stat(full_path, &file_stat) == 0)
		{
			free(path_dup);
			return (full_path);
		}

		free(full_path);
		token = strtok(NULL, ":");
	}

	free(path_dup);
	return (NULL);
}

/**
 * resolve_path - finds the full path of a command
 * @command: command name to find
 *
 * Return: full path string or NULL if not found
 */
char *resolve_path(char *command)
{
	struct stat file_stat;
	char *path_env, *path_dup;

	if (!command)
		return (NULL);

	if (command[0] == '/' || command[0] == '.')
		return (stat(command, &file_stat) == 0 ? strdup(command) : NULL);

	path_env = get_env_value("PATH");
	if (!path_env)
		return (NULL);

	path_dup = strdup(path_env);
	if (!path_dup)
		return (NULL);

	return (search_in_path(path_dup, command));
}

/**
 * str_to_int - converts string to integer
 * @str: string to convert
 *
 * Return: integer value
 */
int str_to_int(char *str)
{
	int num = 0, sign = 1, i = 0;

	if (!str)
		return (0);

	if (str[0] == '-')
	{
		sign = -1;
		i = 1;
	}
	else if (str[0] == '+')
		i = 1;

	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}

	return (num * sign);
}

/**
 * validate_number - checks if string contains valid number
 * @str: string to validate
 *
 * Return: 1 if valid number, 0 otherwise
 */
int validate_number(char *str)
{
	int i = 0;

	if (!str || str[0] == '\0')
		return (0);

	if (str[0] == '-' || str[0] == '+')
		i = 1;

	if (str[i] == '\0')
		return (0);

	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}

	return (1);
}
