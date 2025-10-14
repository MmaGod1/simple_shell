#include "main.h"

/**
 * expand_variables - returns a new malloc'd string with $?, $$, and $VAR expanded
 * @line: input string (NUL terminated)
 * @last_status: last command exit status
 *
 * Return: newly allocated string (caller must free), or NULL on malloc error
 */
char *expand_variables(char *line, int last_status)
{
	char num[32], pid_str[32];
	int len_num, len_pid;
	char *p;
	int needed;
	char *out;
	char *q;
	int i;

	if (!line)
		return (NULL);

	len_num = _int_to_str(last_status, num);
	len_pid = _int_to_str((int)getpid(), pid_str);

	needed = 0;
	p = line;
	while (*p)
	{
		if (*p == '$' && *(p + 1) == '?')
		{
			needed += len_num;
			p += 2;
		}
		else if (*p == '$' && *(p + 1) == '$')
		{
			needed += len_pid;
			p += 2;
		}
		else if (*p == '$' && ((*(p + 1) >= 'A' && *(p + 1) <= 'Z') ||
					(*(p + 1) >= 'a' && *(p + 1) <= 'z') ||
					*(p + 1) == '_'))
		{
			const char *r = p + 1;
			int vlen = 0;
			char varname[256];
			char *val;

			while ((*r >= 'A' && *r <= 'Z') ||
					(*r >= 'a' && *r <= 'z') ||
					(*r >= '0' && *r <= '9') ||
					(*r == '_'))
			{
				if (vlen < 255)
					varname[vlen++] = *r;
				r++;
			}
			varname[vlen] = '\0';
			val = _getenv(varname);
			if (val)
				needed += _strlen(val);
			p = (char *)r;
		}
		else
		{
			needed++;
			p++;
		}
	}

	out = malloc(needed + 1);
	if (!out)
		return (NULL);

	p = line;
	q = out;
	while (*p)
	{
		if (*p == '$' && *(p + 1) == '?')
		{
			for (i = 0; i < len_num; i++)
				*q++ = num[i];
			p += 2;
		}
		else if (*p == '$' && *(p + 1) == '$')
		{
			for (i = 0; i < len_pid; i++)
				*q++ = pid_str[i];
			p += 2;
		}
		else if (*p == '$' && ((*(p + 1) >= 'A' && *(p + 1) <= 'Z') ||
					(*(p + 1) >= 'a' && *(p + 1) <= 'z') ||
					*(p + 1) == '_'))
		{
			const char *r = p + 1;
			int vlen = 0;
			char varname[256];
			char *val;

			while ((*r >= 'A' && *r <= 'Z') ||
					(*r >= 'a' && *r <= 'z') ||
					(*r >= '0' && *r <= '9') ||
					(*r == '_'))
			{
				if (vlen < 255)
					varname[vlen++] = *r;
				r++;
			}
			varname[vlen] = '\0';
			val = _getenv(varname);
			if (val)
			{
				for (i = 0; val[i]; i++)
					*q++ = val[i];
			}
			p = (char *)r;
		}
		else
		{
			*q++ = *p++;
		}
	}

	*q = '\0';
	return (out);
}
