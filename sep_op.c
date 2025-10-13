#include "main.h"

/**
 * parse_line - parse line with separators and operators (;, &&, ||)
 * @line: input line
 * @commands: array to store parsed commands
 * @operators: array to store operators between commands
 *
 * Return: number of commands parsed
 */
int parse_line(char *line, char **commands, char **operators)
{
	int cmd_count = 0, i = 0, start = 0;
	char *temp;
	int len;

	while (line[i])
	{
		/* Check for operators */
		if ((line[i] == ';' || line[i] == '&' || line[i] == '|') && cmd_count < 100)
		{
			len = i - start;
			temp = malloc(len + 1);
			if (temp)
			{
				_strncpy(temp, line + start, len);
				temp[len] = '\0';
				commands[cmd_count++] = temp;
			}

			/* Store operator */
			if (line[i] == ';')
			{
				operators[cmd_count - 1] = ";";
				i++;
			}
			else if (line[i] == '&' && line[i + 1] == '&')
			{
				operators[cmd_count - 1] = "&&";
				i += 2;
			}
			else if (line[i] == '|' && line[i + 1] == '|')
			{
				operators[cmd_count - 1] = "||";
				i += 2;
			}
			else if (line[i] == '|')
			{
				operators[cmd_count - 1] = "|";
				i++;
			}

			/* Skip spaces */
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			start = i;
		}
		else
			i++;
	}

	/* Add last command */
	if (start < i && cmd_count < 100)
	{
		len = i - start;
		temp = malloc(len + 1);
		if (temp)
		{
			_strncpy(temp, line + start, len);
			temp[len] = '\0';
			commands[cmd_count++] = temp;
		}
	}

	return (cmd_count);
}

/**
 * execute_with_operators - execute commands with ; && || operators
 * @line: input line
 * @av: argument vector
 * @status: pointer to last status
 *
 * Return: last command status
 */
int execute_with_operators(char *line, char **av, int *status)
{
	char *commands[100];
	char *operators[100];
	char **args;
	int cmd_count, i;

	cmd_count = parse_line(line, commands, operators);

	if (cmd_count == 0)
		return (0);

	for (i = 0; i < cmd_count; i++)
	{
		args = malloc(sizeof(char *) * 100);
		if (!args)
			return (-1);

		_strtok(commands[i], args, 100, " \n\t");

		if (args[0] == NULL)
		{
			free(args);
			continue;
		}

		/* Check operator from previous command */
		if (i > 0)
		{
			if (_strcmp(operators[i - 1], "&&") == 0 && *status != 0)
			{
				free(args);
				continue; /* Skip if previous failed */
			}
			if (_strcmp(operators[i - 1], "||") == 0 && *status == 0)
			{
				free(args);
				continue; /* Skip if previous succeeded */
			}
		}

		/* Handle builtins */
		if (handle_builtin(args, status))
		{
			free(args);
			continue;
		}

		/* Execute external command */
		*status = shell(args, av);
		free(args);
	}

	/* Free command strings */
	for (i = 0; i < cmd_count; i++)
		free(commands[i]);

	return (*status);
}

