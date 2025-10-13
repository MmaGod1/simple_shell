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

	/* Check for alias substitution */
	alias_t *alias_entry = find_alias(args[0]);
	if (alias_entry)
	{
		free(args[0]);
		args[0] = malloc(_strlen(alias_entry->value) + 1);
		if (args[0])
			_strcpy(args[0], alias_entry->value);
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
	char *cmd_start = line;
	char *op = NULL;
	int result = 0;

	while (*cmd_start)
	{
		char *next = cmd_start;

		/* Find next operator (;, &&, ||) */
		while (*next && !(*next == ';' ||
					(*next == '&' && *(next + 1) == '&') ||
					(*next == '|' && *(next + 1) == '|')))
			next++;

		/* Temporarily terminate current command */
		if (*next)
		{
			if (*next == ';')
				op = ";";
			else if (*next == '&')
				op = "&&";
			else if (*next == '|')
				op = "||";

			*next = '\0';
		}
		else
			op = NULL;

		/* Trim whitespace */
		while (*cmd_start == ' ' || *cmd_start == '\t')
			cmd_start++;

		if (*cmd_start != '\0')
		{
			if (!op || _strcmp(op, ";") == 0 ||
					(_strcmp(op, "&&") == 0 && result == 0) ||
					(_strcmp(op, "||") == 0 && result != 0))
			{
				result = execute_command(cmd_start, av, status);
			}
		}

		/* Move to next command */
		if (!op)
			break;

		if (_strcmp(op, "&&") == 0 || _strcmp(op, "||") == 0)
			cmd_start = next + 2;
		else
			cmd_start = next + 1;
	}

	return (result);
}
