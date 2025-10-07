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
	char *full_path = NULL;

	/* Check if command has '/' or needs PATH search */
	if (args[0][0] != '/')
		full_path = find_path(args[0]);
	else
		full_path = _strdup(args[0]);

	if (full_path == NULL)
	{
		perror(av[0]);
		return (1);
	}

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error");
		free(full_path);
		return (1);
	}

	if (child_pid == 0)
	{
		if (execve(full_path, args, environ) == -1)
		{
			perror(av[0]);
			free(full_path);
			exit(1);
		}
	}
	else
		wait(&status);

	free(full_path);
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
	char *args[64], *line = NULL, *tokens;
	size_t len = 0;
	(void)ac;

	while (1)
	{
		/* Print prompt only in interactive mode */
		if (isatty(STDIN_FILENO))
			printf("($) ");

		if (getline(&line, &len, stdin) == -1)
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			break;
		}

		i = 0;
		tokens = strtok(line, " \n");
		while (tokens != NULL && i < 63)
		{
			args[i++] = tokens;
			tokens = strtok(NULL, " \n");
		}
		args[i] = NULL;

		if (args[0] == NULL)
			continue;

		shell(args, av);
	}
	free(line);
	return (0);
}
