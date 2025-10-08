#include "main.h"

/**
 * print_env - prints the current environment
 *
 * Return: void
 */
void print_env(void)
{
	int i = 0;
	size_t len;

	while (environ[i])
	{
		len = 0;
		while (environ[i][len])
			len++;

		write(STDOUT_FILENO, environ[i], len);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}
