#include "main.h"

/**
 * shell - executes a command in a child process
 * @args: array of arguments, where args[0] is the command
 * @av: argument vector from main (used for program name)
 *
 * Return: exit status (0 on success, 127 if command not found)
 */
int shell(char **args, char **av)
{
	pid_t child_pid;
	int status;
	char *full_path = NULL;

	full_path = find_path(args[0]);
	if (full_path == NULL)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", av[0], args[0]);
		return (127);
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
	{
		waitpid(child_pid, &status, 0);
	}

	free(full_path);

	/* Ensure the shell returns the correct status from child */
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	return (status);
}

/**
 * main - entry point for the simple shell
 * @ac: argument count
 * @av: argument vector
 *
 * Return: Always 0
 */
int main(int ac, char *av[])
{
	int i, status = 0;
	char *args[64], *line = NULL, *tokens;
	size_t len = 0;
	(void)ac;

	while (1)
	{
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

		status = shell(args, av);
	}
	free(line);
	return (status == 127 ? 127 : 0);
}
