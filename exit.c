#include "main.h"

/**
 * handle_exit - handles the 'exit' built-in command
 * @args: array of arguments (e.g. {"exit", "98", NULL})
 * @line: pointer to malloc'd input line (will be freed)
 * @status: current shell status
 * @av: argument vector of main (used for error printing)
 *
 * Description:
 *  - If no argument is given, exits with current status.
 *  - If numeric argument is given, exits with that value.
 *  - If invalid number, prints error and returns 1 (don’t exit).
 *
 * Return: 1 if should continue shell loop, 0 if exited
 */
int handle_exit(char **args, char *line, int status, char **av)
{
	int i, exit_status = status;

	/* No  argument */
	if (args[1] == NULL)
	{
		free(line);
		exit(exit_status);
	}

	/* Check if args[1] is numeric */
	for (i = 0; args[1][i] != '\0'; i++)
	{
		if (args[1][i] < '0' || args[1][i] > '9')
		{
			fprintf(stderr, "%s: 1: exit: Illegal number: %s\n", av[0], args[1]);
			free(line);
			return (1); /* continue shell */
		}
	}

	exit_status = _atoi(args[1]);
	free(line);
	exit(exit_status);
}
