#include "shell.h"

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
