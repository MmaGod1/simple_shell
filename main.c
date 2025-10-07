#include "main.h"

/**
 * shell - creates a child process and executes a command
 * @args: array of arguments, where args[0] is the command
 * @av: argument vector from main, used for printing program name
 *
 * Return: 0 on success, 1 on error
 */
int shell(char **args, char **av)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error");
		return (1);
	}

	if (child_pid == 0)
	{
		if (execve(args[0], args, environ) == -1)
		{
			perror(av[0]);
			exit(1);
		}
	}
	else
		wait(&status);

	return (0);
}

/**
 * main - entry point for the simple shell
 * @ac: argument count
 * @av: argument vector
 *
 * Return: Always 0 (Success)
 */
int main(int ac, char *av[])
{
	int i = 0;
	char *args[64];
	char *line = NULL, *tokens;
	size_t len = 0;

	(void)ac;

	while (1)
	{
		printf("($) ");
		if (getline(&line, &len, stdin) == -1)
		{
			printf("\n");
			break;
		}

		i = 0;
		tokens = strtok(line, " \n");
		while (tokens != NULL && i < 64)
		{
			args[i++] = tokens;
			tokens = strtok(NULL, " \n");
		}
		args[i] = NULL;

		if (args[0] == NULL)
			continue;

		if (args[1] != NULL)
		{
			fprintf(stderr, "%s: No such file or directory\n", av[0]);
			continue;
		}

		shell(args, av);
	}

	free(line);
	return (0);
}
