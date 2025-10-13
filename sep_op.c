#include "main.h"

/**
 * execute_command - execute a single command
 * @cmd: command string to execute
 * @av: argument vector
 * @status: pointer to last status
 *
 * Return: command status
 */
int execute_command(char *cmd, char **av, int *status)
{
	char **args;
	int result;

	args = malloc(sizeof(char *) * 1024);
	if (!args)
		return (-1);

	_strtok(cmd, args, 1024, " \n\t");

	if (args[0] == NULL)
	{
		free(args);
		return (0);
	}

	/* Handle builtins */
	if (handle_builtin(args, status))
	{
		free(args);
		return (*status);
	}

	/* Execute external command */
	result = shell(args, av);
	*status = result;
	free(args);
	return (result);
}

/**
 * execute_with_operators - parse and execute commands with ;, &&, ||
 * @line: input line
 * @av: argument vector
 * @status: pointer to last status
 *
 * Return: last command status
 */
int execute_with_operators(char *line, char **av, int *status)
{
	char *cmd_copy, *cmd, *sep_pos;
	char *current_op = ";";
	int result = 0;

	cmd_copy = malloc(_strlen(line) + 1);
	if (!cmd_copy)
		return (-1);
	_strcpy(cmd_copy, line);

	cmd = cmd_copy;

	while (*cmd)
	{
		/* Skip leading whitespace */
		while (*cmd == ' ' || *cmd == '\t')
			cmd++;

		/* Find next operator */
		sep_pos = cmd;
		while (*sep_pos && *sep_pos != ';' && 
				!(*sep_pos == '&' && *(sep_pos + 1) == '&') &&
				!(*sep_pos == '|' && *(sep_pos + 1) == '|'))
			sep_pos++;

		if (*sep_pos)
		{
			*sep_pos = '\0';

			/* Check what operator we found */
			if (*(sep_pos + 1) == '&')
				current_op = "&&";
			else if (*(sep_pos + 1) == '|')
				current_op = "||";
			else
				current_op = ";";
		}

		/* Execute current command based on previous operator */
		if (_strcmp(current_op, ";") == 0 || 
				(_strcmp(current_op, "&&") == 0 && result == 0) ||
				(_strcmp(current_op, "||") == 0 && result != 0))
		{
			result = execute_command(cmd, av, status);
		}

		/* Move to next command */
		if (*sep_pos)
		{
			if (_strcmp(current_op, "&&") == 0 || 
					_strcmp(current_op, "||") == 0)
				cmd = sep_pos + 2;
			else
				cmd = sep_pos + 1;
		}
		else
			break;
	}

	free(cmd_copy);
	return (result);
}

