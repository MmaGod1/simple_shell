#include "main.h"

/**
 * _getenv - get environment variable value
 * @name: variable name
 *
 * Return: pointer to value string, or NULL if not found
 */
char *_getenv(const char *name)
{
	int i, j;
	size_t name_len;

	if (!name || !environ)
		return (NULL);

	name_len = _strlen((char *)name);

	for (i = 0; environ[i]; i++)
	{
		for (j = 0; j < (int)name_len && environ[i][j] == name[j]; j++)
			;

		if (j == (int)name_len && environ[i][j] == '=')
			return (&environ[i][j + 1]);
	}

	return (NULL);
}

/**
 * _setenv - set or modify environment variable
 * @name: variable name
 * @value: variable value
 * @overwrite: whether to overwrite existing variable
 *
 * Return: 0 on success, -1 on failure
 */
int _setenv(const char *name, const char *value, int overwrite)
{
	char **new_environ, *new_var;
	int i, j, k, name_len, value_len, env_size = 0;

	if (!name || !value || name[0] == '\0')
		return (-1);

	name_len = _strlen((char *)name);
	value_len = _strlen((char *)value);

	/* Check if variable exists */
	for (i = 0; environ[i]; i++)
	{
		for (j = 0; j < name_len && environ[i][j] == name[j]; j++)
			;
		if (j == name_len && environ[i][j] == '=')
		{
			if (!overwrite)
				return (0);
			/* Replace existing variable */
			new_var = malloc(name_len + value_len + 2);
			if (!new_var)
				return (-1);
			for (j = 0; j < name_len; j++)
				new_var[j] = name[j];
			new_var[j++] = '=';
			for (k = 0; k < value_len; k++)
				new_var[j++] = value[k];
			new_var[j] = '\0';
			environ[i] = new_var;
			return (0);
		}
		env_size++;
	}

	/* Add new variable */
	new_environ = malloc(sizeof(char *) * (env_size + 2));
	if (!new_environ)
		return (-1);
	for (i = 0; i < env_size; i++)
		new_environ[i] = environ[i];

	new_var = malloc(name_len + value_len + 2);
	if (!new_var)
	{
		free(new_environ);
		return (-1);
	}
	for (j = 0; j < name_len; j++)
		new_var[j] = name[j];
	new_var[j++] = '=';
	for (k = 0; k < value_len; k++)
		new_var[j++] = value[k];
	new_var[j] = '\0';

	new_environ[env_size] = new_var;
	new_environ[env_size + 1] = NULL;
	environ = new_environ;
	return (0);
}

/**
 * _unsetenv - remove environment variable
 * @name: variable name
 *
 * Return: 0 on success, -1 on failure
 */
int _unsetenv(const char *name)
{
	int i, j, name_len, found = -1, env_size = 0;

	if (!name || name[0] == '\0')
		return (-1);

	name_len = _strlen((char *)name);

	for (i = 0; environ[i]; i++)
	{
		for (j = 0; j < name_len && environ[i][j] == name[j]; j++)
			;
		if (j == name_len && environ[i][j] == '=')
			found = i;
		env_size++;
	}

	if (found == -1)
		return (0);

	/* Shift remaining variables */
	for (i = found; i < env_size - 1; i++)
		environ[i] = environ[i + 1];
	environ[env_size - 1] = NULL;

	return (0);
}

/**
 * handle_setenv - built-in setenv command
 * @args: command arguments
 *
 * Return: 0 always (even on error, per test requirements)
 */
int handle_setenv(char **args)
{
	if (!args[1] || !args[2])
	{
		fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		return (0);
	}

	_setenv(args[1], args[2], 1);
	return (0);
}

/**
 * handle_unsetenv - built-in unsetenv command
 * @args: command arguments
 *
 * Return: 0 always
 */
int handle_unsetenv(char **args)
{
	if (!args[1])
	{
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		return (0);
	}

	_unsetenv(args[1]);
	return (0);
}

