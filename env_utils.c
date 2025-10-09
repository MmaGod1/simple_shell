#include "main.h"

/**
 * handle_setenv - built-in implementation of setenv
 * @args: command arguments
 *
 * Description:
 *   Usage: setenv VARIABLE VALUE
 *   If VARIABLE or VALUE is missing, prints an error to stderr.
 *
 * Return: 0 on success, 1 on failure
 */
int handle_setenv(char **args)
{
	if (!args[1] || !args[2])
	{
		fprintf(stderr, "setenv: Usage: setenv VARIABLE VALUE\n");
		return (1);
	}

	if (setenv(args[1], args[2], 1) != 0)
	{
		perror("setenv");
		return (1);
	}

	return (0);
}

/**
 * handle_unsetenv - built-in implementation of unsetenv
 * @args: command arguments
 *
 * Description:
 *   Usage: unsetenv VARIABLE
 *   Removes the specified environment variable.
 *
 * Return: 0 on success, 1 on failure
 */
int handle_unsetenv(char **args)
{
	if (!args[1])
	{
		fprintf(stderr, "unsetenv: Usage: unsetenv VARIABLE\n");
		return (1);
	}

	if (unsetenv(args[1]) != 0)
	{
		perror("unsetenv");
		return (1);
	}

	return (0);
}
