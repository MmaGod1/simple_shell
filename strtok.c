#include "main.h"

/**
 * is_delimiter - checks if character is a delimiter
 * @c: character to check
 * @delim: string of delimiter characters
 *
 * Return: 1 if delimiter, 0 otherwise
 */
int is_delimiter(char c, const char *delim)
{
	int i;

	for (i = 0; delim[i]; i++)
	{
		if (c == delim[i])
			return (1);
	}
	return (0);
}

/**
 * strtok - splits a line into tokens
 * @line: the line to tokenize
 * @args: array to store token pointers
 * @max_args: maximum number of arguments
 * @delim: delimiter characters (e.g., " \n\t")
 *
 * Return: number of tokens found
 */
int _strtok(char *line, char **args, int max_args, const char *delim)
{
	int i = 0, j = 0, in_token = 0;

	if (!line || !args)
		return (0);

	while (line[j] && i < max_args - 1)
	{
		/* Skip leading delimiters */
		if (is_delimiter(line[j], delim))
		{
			if (in_token)
			{
				line[j] = '\0';
				in_token = 0;
			}
			j++;
		}
		else
		{
			if (!in_token)
			{
				args[i++] = &line[j];
				in_token = 1;
			}
			j++;
		}
	}

	args[i] = NULL;
	return (i);
}

