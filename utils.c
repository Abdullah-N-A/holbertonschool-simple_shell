#include "shell.h"

/**
 * get_env_value - retrieves environment variable value
 * @name: name of environment variable
 *
 * Return: pointer to value or NULL
 */
char *get_env_value(char *name)
{
	int i, j, len;

	if (!name || !environ)
		return (NULL);

	len = strlen(name);

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
 * resolve_path - finds the full path of a command
 * @command: command name to find
 *
 * Return: full path string or NULL if not found
 */
char *resolve_path(char *command)
{
	struct stat file_stat;
	char *path_env, *path_dup, *token, *full_path;
	int cmd_len, dir_len;

	if (!command)
		return (NULL);

	if (command[0] == '/' || command[0] == '.')
		return (stat(command, &file_stat) == 0 ? strdup(command) : NULL);

	path_env = get_env_value("PATH");
	if (!path_env)
		return (NULL);

	path_dup = strdup(path_env);
	cmd_len = strlen(command);
	token = strtok(path_dup, ":");

	while (token)
	{
		dir_len = strlen(token);
		full_path = malloc(dir_len + cmd_len + 2);
		if (!full_path)
		{
			free(path_dup);
			return (NULL);
		}

		strcpy(full_path, token);
		full_path[dir_len] = '/';
		strcpy(full_path + dir_len + 1, command);

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
 * str_to_int - converts string to integer (custom atoi)
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
