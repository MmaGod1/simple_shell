#include "main.h"

/**
 * _atoi - converts a string to an integer
 * @s: string to convert
 * Return: integer value
 */
int _atoi(char *s)
{
	int i = 0, sign = 1, result = 0;

	if (s == NULL)
		return (0);

	if (s[0] == '-')
	{
		sign = -1;
		i++;
	}

	while (s[i] != '\0')
	{
		if (s[i] >= '0' && s[i] <= '9')
			result = result * 10 + (s[i] - '0');
		else
			break;
		i++;
	}

	return (sign * result);
}


/**
 * _strncmp - compares two strings up to n characters
 * @s1: first string
 * @s2: second string
 * @n: number of characters to compare
 * Return: 0 if equal, difference otherwise
 */
int _strncmp(char *s1, char *s2, int n)
{
	int i;

	for (i = 0; i < n && s1[i] && s2[i]; i++)
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
	}

	if (i < n)
		return (s1[i] - s2[i]);

	return (0);
}

/**
 * _strchr - locates the first occurrence of a character in a string  
 * @s: the string to search  
 * @c: the character to locate  
 * Return: a pointer to the first occurrence of @c in @s, or NULL if not found  
 */
char *_strchr(const char *s, char c)
{
	while (*s && *s != c)
		s++;
	return (*s == c ? (char *)s : NULL);
}


/**
 * _strncpy - copies up to n characters from one string to another  
 * @dest: destination buffer  
 * @src: source string  
 * @n: maximum number of characters to copy  
 * Return: a pointer to the destination string @dest  
 */
void _strncpy(char *dest, const char *src, size_t n) {
	size_t i;
	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];
	for (; i < n; i++)
		dest[i] = '\0';
}


/**
 * _int_to_str - convert integer to string placed in buf (no malloc)
 * @n: integer to convert
 * @buf: buffer to write
 *
 * Return: number of digits written
 */
int _int_to_str(int n, char *buf)
{
	int len = 0;
	int i;
	char rev[32];
	int tmp;

	if (n == 0)
	{
		buf[0] = '0';
		return (1);
	}

	tmp = n;
	while (tmp > 0)
	{
		rev[len++] = (char)('0' + (tmp % 10));
		tmp /= 10;
	}

	for (i = 0; i < len; i++)
		buf[i] = rev[len - 1 - i];

	return (len);
}
