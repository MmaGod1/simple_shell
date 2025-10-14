#include "main.h"

#define READ_BUF_SIZE 1024
#define INITIAL_LINE_SIZE 128

/**
 * _getline - reads a line from stdin efficiently
 * Return: pointer to the line (malloc'd), or NULL on EOF/error
 */
char *_getline(void)
{
	static char buffer[READ_BUF_SIZE];
	static ssize_t buf_pos, buf_size;
	char *line = NULL, *new_line;
	size_t line_len = 0, line_capacity = INITIAL_LINE_SIZE;
	char c;
	size_t i;

	line = malloc(line_capacity);
	if (!line)
		return (NULL);

	while (1)
	{
		/* refill buffer if empty */
		if (buf_pos >= buf_size)
		{
			buf_size = read(STDIN_FILENO, buffer, READ_BUF_SIZE);
			buf_pos = 0;
			if (buf_size <= 0)
			{
				if (line_len == 0)
				{
					free(line);
					return (NULL);
				}
				break;
			}
		}

		c = buffer[buf_pos++];

		/* expand buffer if needed (double the size) */
		if (line_len + 1 >= line_capacity)
		{
			line_capacity *= 2;
			new_line = malloc(line_capacity);
			if (!new_line)
			{
				free(line);
				return (NULL);
			}

			/* copy existing content */
			for (i = 0; i < line_len; i++)
				new_line[i] = line[i];

			free(line);
			line = new_line;
		}

		line[line_len++] = c;

		if (c == '\n')
			break;
	}

	line[line_len] = '\0';
	return (line);
}

/**
 * _getline_fd - reads one line from a file descriptor
 * @fd: file descriptor
 * Return: line read or NULL on EOF/error
 */
char *_getline_fd(int fd)
{
	char *buf = NULL;
	char c;
	int rd, i = 0, size = 1024;

	buf = malloc(size);
	if (!buf)
		return (NULL);

	while ((rd = read(fd, &c, 1)) > 0)
	{
		if (c == '\n')
			break;

		buf[i++] = c;
		if (i >= size - 1)
		{
			size += 1024;
			buf = _realloc(buf, size - 1024, size);
			if (!buf)
				return (NULL);
		}
	}

	if (rd == 0 && i == 0)
	{
		free(buf);
		return (NULL);
	}

	buf[i] = '\0';
	return (buf);
}
