#include "main.h"

/**
 * handle_exit - handles the 'exit' built-in command
 * @args: array of arguments (e.g. {"exit", "98", NULL})
 * @line: pointer to malloc'd input line (will be freed)
 * @status: current shell status
 * @av: argument vector of main (used for error printing)
 *
 * Description:
 *  - If no argument: exits with current status.
 *  - If argument is a valid integer: exits with that code.
 *  - If invalid number: prints error and exits with status 2.
 *
 * Return: Does not return if exiting; returns 1 to continue otherwise.
 */
int handle_exit(char **args, char *line, int status, char **av)
{
	int i = 0, exit_status = status;
	char *num = args[1];

	/* No argument */
	if (num == NULL)
	{
		free(line);
		exit(exit_status);
	}

	/* Check if numeric argument (may start with '-') */
	if (num[0] == '-')
		i++;

	for (; num[i] != '\0'; i++)
	{
		if (num[i] < '0' || num[i] > '9')
		{
			fprintf(stderr, "%s: 1: exit: Illegal number: %s\n", av[0], num);
			free(line);
			exit(2); /* required by checker */
		}
	}

	exit_status = _atoi(num);
	free(line);
	exit(exit_status);
}
