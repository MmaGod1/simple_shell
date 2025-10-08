#include "main.h"
#include <unistd.h>
#include <stdlib.h>

#define READ_BUF_SIZE 1024

/**
 * _getline - reads a line from stdin
 *
 * Return: pointer to the line (malloc’d), or NULL on EOF/error
 */
char *_getline(void)
{
	static char buffer[READ_BUF_SIZE];
	static ssize_t buf_pos, buf_size;
	char *line = NULL, *new_line;
	size_t line_len = 0;
	char c;
	ssize_t j;

	while (1)
	{
		/* refill buffer if empty */
		if (buf_pos >= buf_size)
		{
			buf_size = read(STDIN_FILENO, buffer, READ_BUF_SIZE);
			buf_pos = 0;
			if (buf_size <= 0)
			{
				free(line);
				return (NULL);
			}
		}

		/* read one char */
		c = buffer[buf_pos++];

		/* allocate new space manually */
		new_line = malloc(line_len + 2);
		if (!new_line)
		{
			free(line);
			return (NULL);
		}

		/* manually copy old content */
		for (j = 0; j < (ssize_t)line_len; j++)
			new_line[j] = line[j];

		if (line)
			free(line);

		line = new_line;
		line[line_len++] = c;
		line[line_len] = '\0';

		/* stop on newline */
		if (c == '\n')
			break;
	}

	return (line);
}
