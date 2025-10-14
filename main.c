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
			exit(2);
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

	return (2);
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
	int status = 0;
	char *line = NULL;
	FILE *fp;

	if (ac == 2)
	{
		fp = fopen(av[1], "r");
		if (!fp)
		{
			perror(av[0]);
			return (EXIT_FAILURE);
		}

		while ((line = _getline_file(fp)) != NULL)
		{
			remove_comment(line);
			if (line[0] == '\0')
			{
				free(line);
				continue;
			}

			execute_with_operators(line, av, &status);
			free(line);
		}

		fclose(fp);
		free_env();
		free_aliases();
		return (status);
	}

	/* Interactive mode (already perfect) */
	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			printf("($) ");
			fflush(stdout);
		}

		line = _getline();
		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			break;
		}

		if (line[0] == '\0')
		{
			free(line);
			continue;
		}

		if (_strncmp(line, "exit", 4) == 0)
		{
			char *args[1024];
			_strtok(line, args, 1024, " \n\t");
			if (handle_exit(args, line, status, av))
				continue;
		}

		remove_comment(line);
		execute_with_operators(line, av, &status);
		free(line);
	}

	free_env();
	free_aliases();
	return (status);
}
