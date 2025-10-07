#include "main.h"

/**
 * _get_path - finds the PATH variable in environ
 *
 * Return: pointer to the PATH string (after 'PATH='),
 * or NULL if not found
 */
char *_get_path(void)
{
	int i = 0;

	while (environ[i])
	{
		if (environ[i][0] == 'P' &&
				environ[i][1] == 'A' &&
				environ[i][2] == 'T' &&
				environ[i][3] == 'H' &&
				environ[i][4] == '=')
		{
			return (environ[i] + 5);
		}
		i++;
	}
	return (NULL);
}
