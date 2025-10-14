#include "main.h"

/**
 * execute_command - execute a single command
 * @cmd: command string to execute
 * @av: argument vector
 * @status: pointer to last status
 * Return: command exit status
 */
int execute_command(char *cmd, char **av, int *status)
{
	char **args;
	int result;
	alias_t *alias_entry;
	char *alloc_name;
	int alias_depth;

	args = malloc(sizeof(char *) * 1024);
	if (!args)
		return (-1);

	_strtok(cmd, args, 1024, " \n\t");

	if (args[0] == NULL)
	{
		free(args);
		return (0);
	}

	/* recursive alias expansion, allocate replacements only */
	alloc_name = NULL;
	alias_depth = 0;
	alias_entry = find_alias(args[0]);

	while (alias_entry && alias_depth < 10)
	{
		char *val;
		char *new_alloc;

		val = alias_entry->value;

		/* stop on self reference */
		if (_strcmp(val, args[0]) == 0)
			break;

		new_alloc = malloc(_strlen(val) + 1);
		if (!new_alloc)
			break;

		_strcpy(new_alloc, val);

		/* free previous allocated alias replacement, if any */
		if (alloc_name)
		{
			free(alloc_name);
			alloc_name = NULL;
		}

		alloc_name = new_alloc;
		args[0] = alloc_name;

		alias_depth++;
		alias_entry = find_alias(args[0]);
	}

	/* handle builtins (must free alloc_name if allocated) */
	if (handle_builtin(args, status))
	{
		if (alloc_name)
			free(alloc_name);
		free(args);
		return (*status);
	}

	/* execute external command */
	result = shell(args, av);

	if (WIFEXITED(result))
		result = WEXITSTATUS(result);

	*status = result;

	if (alloc_name)
		free(alloc_name);
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
	char *cmd;
	char *next;
	char *save;
	char op_next[3];
	char op_prev[3];
	char *segment;
	int result;
	int len;

	cmd = line;
	_strcpy(op_prev, ";");   /* previous operator (controls execution) */
	_strcpy(op_next, ";");
	result = 0;

	while (cmd && *cmd)
	{
		while (*cmd == ' ' || *cmd == '\t')
			cmd++;

		next = cmd;
		while (*next && !(*next == ';' ||
					(*next == '&' && *(next + 1) == '&') ||
					(*next == '|' && *(next + 1) == '|')))
			next++;

		save = next;

		/* determine operator that follows this command (op_next) */
		if (*next)
		{
			if (*next == ';')
				_strcpy(op_next, ";");
			else if (*next == '&')
				_strcpy(op_next, "&&");
			else if (*next == '|')
				_strcpy(op_next, "||");
		}
		else
			_strcpy(op_next, ";");

		/* copy current command into independent buffer */
		len = next - cmd;
		segment = malloc(len + 1);
		if (!segment)
			return (-1);
		_strncpy(segment, cmd, (size_t)len);
		segment[len] = '\0';

		/* decide execution using previous operator and previous result */
		if (_strcmp(op_prev, ";") == 0 ||
				(_strcmp(op_prev, "&&") == 0 && result == 0) ||
				(_strcmp(op_prev, "||") == 0 && result != 0))
		{
			expand_status(segment, *status);
			result = execute_command(segment, av, status);
			*status = result;
		}

		free(segment);

		/* advance: previous operator becomes operator that followed this cmd */
		_strcpy(op_prev, op_next);

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
