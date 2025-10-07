#include "main.h"

/**
 * find_path - searches for an executable in PATH
 * @command: command name entered by the user
 *
 * Return: pointer to the full path (malloc’d) if found, NULL otherwise
 */
char *find_path(char *command)
{
	char *path_env, *path_copy, *dir, *full_path;
	size_t len, i;

	/* check if command already has a slash */
	for (i = 0; command[i] != '\0'; i++)
	{
		if (command[i] == '/')
		{
			if (access(command, X_OK) == 0)
				return (_strdup(command));
			else
				return (NULL);
		}
	}

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);

	path_copy = _strdup(path_env);
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir)
	{
		len = _strlen(dir) + _strlen(command) + 2;
		full_path = malloc(len);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}

		_strcpy(full_path, dir);
		_strcat(full_path, "/");
		_strcat(full_path, command);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}
