#include "main.h"

/* Keep track of dynamically allocated environment variables */
static char **allocated_vars = NULL;
static int allocated_count = 0;

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
 * _setenv - sets or updates an environment variable
 * @name: name of the variable
 * @value: value to set
 * @overwrite: if non-zero, overwrite existing value
 * Return: 0 on success, -1 on failure
 */
int _setenv(const char *name, const char *value, int overwrite)
{
	int i;
	size_t name_len, value_len;
	char *new_var;
	char **tmp;

	if (!name || !value)
		return (-1);

	name_len = _strlen((char *)name);
	value_len = _strlen((char *)value);

	/* Check if variable already exists */
	for (i = 0; environ[i]; i++)
	{
		if (_strncmp(environ[i], (char *)name, name_len) == 0 &&
				environ[i][name_len] == '=')
		{
			if (!overwrite)
				return (0);

			new_var = malloc(name_len + value_len + 2);
			if (!new_var)
				return (-1);

			_strcpy(new_var, (char *)name);
			new_var[name_len] = '=';
			_strcpy(new_var + name_len + 1, (char *)value);

			free(environ[i]);
			environ[i] = new_var;

			/* Don't add to allocated_vars for updates - only for new vars */
			return (0);
		}
	}

	/* Add new variable */
	new_var = malloc(name_len + value_len + 2);
	if (!new_var)
		return (-1);

	_strcpy(new_var, (char *)name);
	new_var[name_len] = '=';
	_strcpy(new_var + name_len + 1, (char *)value);

	tmp = realloc(allocated_vars,
			sizeof(char *) * (allocated_count + 1));
	if (tmp)
	{
		allocated_vars = tmp;
		allocated_vars[allocated_count++] = new_var;
	}

	environ[i] = new_var;
	environ[i + 1] = NULL;
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


/**
 * free_env - frees all dynamically allocated environment variables
 *
 * Return: void
 */
void free_env(void)
{
	int i;

	for (i = 0; i < allocated_count; i++)
		free(allocated_vars[i]);

	free(allocated_vars);
	allocated_vars = NULL;
	allocated_count = 0;
}
