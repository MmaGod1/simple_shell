#include "main.h"

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
	char *line = NULL;
	size_t line_len = 0;
	char c;

	while (1)
	{
		/* Refill buffer if empty */
		if (buf_pos >= buf_size)
		{
			buf_size = read(STDIN_FILENO, buffer, READ_BUF_SIZE);
			buf_pos = 0;
			if (buf_size <= 0) /* EOF or error */
				return (line);
		}

		/* Get current character */
		c = buffer[buf_pos++];

		/* Append character to the line */
		line = realloc(line, line_len + 2);
		if (!line)
			return (NULL);
		line[line_len++] = c;
		line[line_len] = '\0';

		/* Stop at newline */
		if (c == '\n')
			break;
	}

	return (line);
}
