#include "main.h"

/**
 * _atoi - converts a string to an integer
 * @s: string to convert
 * Return: integer value
 */
int _atoi(char *s)
{
	int i = 0, sign = 1, result = 0;

	if (s == NULL)
		return (0);

	if (s[0] == '-')
	{
		sign = -1;
		i++;
	}

	while (s[i] != '\0')
	{
		if (s[i] >= '0' && s[i] <= '9')
			result = result * 10 + (s[i] - '0');
		else
			break;
		i++;
	}

	return (sign * result);
}


/**
 * _strncmp - compares two strings up to n characters
 * @s1: first string
 * @s2: second string
 * @n: number of characters to compare
 * Return: 0 if equal, difference otherwise
 */
int _strncmp(char *s1, char *s2, int n)
{
	int i;

	for (i = 0; i < n && s1[i] && s2[i]; i++)
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
	}

	if (i < n)
		return (s1[i] - s2[i]);

	return (0);
}
