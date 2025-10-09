#include "main.h"

/**
 * handle_setenv - built-in setenv command
 * @args: command arguments (args[0]="setenv", args[1]=VAR, args[2]=VALUE)
 *
 * Return: 0 on success, 1 on failure
 */
int handle_setenv(char **args)
{
	if (!args[1] || !args[2])
	{
		fprintf(stderr, "setenv: missing arguments\n");
		return (1);
	}

	if (setenv(args[1], args[2], 1) != 0)
	{
		fprintf(stderr, "setenv: failed to set variable\n");
		return (1);
	}

	return (0);
}

/**
 * handle_unsetenv - built-in unsetenv command
 * @args: command arguments (args[0]="unsetenv", args[1]=VAR)
 *
 * Return: 0 on success, 1 on failure
 */
int handle_unsetenv(char **args)
{
	if (!args[1])
	{
		fprintf(stderr, "unsetenv: missing argument\n");
		return (1);
	}

	if (unsetenv(args[1]) != 0)
	{
		fprintf(stderr, "unsetenv: failed to unset variable\n");
		return (1);
	}

	return (0);
}
