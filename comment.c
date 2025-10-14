#include "main.h"
/**
 * remove_comment - removes comment text from a command line
 * @line: input command line
 * Return: pointer to modified line
 */
char *remove_comment(char *line)
{
	int i;
	int in_single = 0, in_double = 0;

	for (i = 0; line[i]; i++)
	{
		if (line[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[i] == '"' && !in_single)
			in_double = !in_double;
		else if (line[i] == '#' && !in_single && !in_double)
		{
			if (i == 0 || line[i - 1] == ' ' || line[i - 1] == '\t')
			{
				line[i] = '\0';
				break;
			}
		}
	}
	return (line);
}
