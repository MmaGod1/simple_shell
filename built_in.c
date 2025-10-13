#include "main.h"

/**
 * handle_cd - built-in cd command
 * @args: command arguments
 *
 * Return: 0 on success, 1 on failure
 */
int handle_cd(char **args)
{
	char *dir, *oldpwd_value, old_cwd[1024], cwd[1024];

	if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
		return (1);

	if (!args[1])
	{
		dir = _getenv("HOME");
		if (!dir)
			return (0); /* Do nothing if HOME not set */
	}
	else if (_strcmp(args[1], "-") == 0)
	{
		oldpwd_value = _getenv("OLDPWD");
		if (!oldpwd_value)
		{
			/* If OLDPWD not set, use current directory */
			dir = old_cwd;
		}
		else
		{
			dir = oldpwd_value;
		}
		printf("%s\n", dir);
	}
	else
	{
		dir = args[1];
	}

	if (chdir(dir) != 0)
	{
		fprintf(stderr, "./hsh: 1: cd: can't cd to %s\n", dir);
		return (1);
	}

	_setenv("OLDPWD", old_cwd, 1);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		_setenv("PWD", cwd, 1);

	return (0);
}


/**
 * handle_builtin - checks and executes builtin commands
 * @args: command arguments
 * @status: pointer to last command status
 *
 * Return: 1 if builtin was handled, 0 if not a builtin
 */
int handle_builtin(char **args, int *status)
{
	if (_strcmp(args[0], "exit") == 0)
		exit(*status);
	else if (_strcmp(args[0], "env") == 0)
	{
		print_env();
		*status = 0;
		return (1);
	}
	else if (_strcmp(args[0], "setenv") == 0)
	{
		*status = handle_setenv(args);
		return (1);
	}
	else if (_strcmp(args[0], "unsetenv") == 0)
	{
		*status = handle_unsetenv(args);
		return (1);
	}
	else if (_strcmp(args[0], "cd") == 0)
	{
		*status = handle_cd(args);
		return (1);
	}

	return (0);
}
