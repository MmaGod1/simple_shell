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
	alias_t *alias_entry;

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
	alias_entry = find_alias(args[0]);
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
 * expand_status - replaces $? with last command status
 * @line: command line to modify
 * @last_status: last command exit status
 * Return: void
 */
void expand_status(char *line, int last_status)
{
	char buffer[2048], num[16];
	char *p = line, *b = buffer;
	int num_len;

	sprintf(num, "%d", last_status);
	num_len = _strlen(num);

	while (*p)
	{
		if (*p == '$' && *(p + 1) == '?')
		{
			_strcpy(b, num);
			b += num_len;
			p += 2;
		}
		else
			*b++ = *p++;
	}

	*b = '\0';
	_strcpy(line, buffer);
}


/**
 * execute_with_operators - execute commands separated by ;, &&, or ||
 * @line: input command line
 * @av: argument vector
 * @status: pointer to last command status
 * Return: status of last executed command
 */
int execute_with_operators(char *line, char **av, int *status)
{
	char *cmd, *next, *save, op[3];
	int result;
	int len;
	char *segment;

	cmd = line;
	_strcpy(op, ";");
	result = 0;

	while (cmd && *cmd)
	{
		/* skip leading whitespace */
		while (*cmd == ' ' || *cmd == '\t')
			cmd++;

		/* find next operator */
		next = cmd;
		while (*next && !(*next == ';' ||
					(*next == '&' && *(next + 1) == '&') ||
					(*next == '|' && *(next + 1) == '|')))
			next++;

		save = next;

		/* determine operator type (do not modify line) */
		if (*next)
		{
			if (*next == ';')
				_strcpy(op, ";");
			else if (*next == '&')
				_strcpy(op, "&&");
			else if (*next == '|')
				_strcpy(op, "||");
		}
		else
			_strcpy(op, ";");

		/* copy current command segment into its own buffer */
		len = next - cmd;
		segment = malloc((len + 1) * sizeof(char));
		if (segment == NULL)
			return (-1);
		_strncpy(segment, cmd, (size_t)len);
		segment[len] = '\0';

		/* execute if operator logic allows */
		if (_strcmp(op, ";") == 0 ||
				(_strcmp(op, "&&") == 0 && result == 0) ||
				(_strcmp(op, "||") == 0 && result != 0))
		{
			expand_status(segment, *status);
			result = execute_command(segment, av, status);
			*status = result;
		}

		free(segment);

		/* advance cmd pointer past the operator */
		if (*save)
		{
			if (*save == '&' || *save == '|')
				cmd = save + 2;
			else
				cmd = save + 1;
		}
		else
			break;
	}

	return (result);
}
