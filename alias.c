#include "main.h"

static alias_t aliases[100];
static int alias_count = 0;

/**
 * find_alias - find an alias by name
 * @name: alias name
 *
 * Return: pointer to alias, or NULL if not found
 */
alias_t *find_alias(const char *name)
{
	int i;

	for (i = 0; i < alias_count; i++)
	{
		if (_strcmp(aliases[i].name, (char *)name) == 0)
			return (&aliases[i]);
	}
	return (NULL);
}

/**
 * set_alias - create or update an alias
 * @name: alias name
 * @value: alias value
 *
 * Return: 0 on success, -1 on failure
 */
int set_alias(const char *name, const char *value)
{
	alias_t *existing;

	existing = find_alias(name);
	if (existing)
	{
		free(existing->value);
		existing->value = malloc(_strlen((char *)value) + 1);
		if (!existing->value)
			return (-1);
		_strcpy(existing->value, (char *)value);
		return (0);
	}

	if (alias_count >= 100)
		return (-1);

	aliases[alias_count].name = malloc(_strlen((char *)name) + 1);
	aliases[alias_count].value = malloc(_strlen((char *)value) + 1);

	if (!aliases[alias_count].name || !aliases[alias_count].value)
		return (-1);

	_strcpy(aliases[alias_count].name, (char *)name);
	_strcpy(aliases[alias_count].value, (char *)value);
	alias_count++;

	return (0);
}

/**
 * print_alias - print a single alias
 * @alias: pointer to alias
 */
void print_alias(alias_t *alias)
{
	printf("%s='%s'\n", alias->name, alias->value);
}

/**
 * handle_alias - built-in alias command
 * @args: command arguments
 *
 * Return: 0 on success, 1 on failure
 */
int handle_alias(char **args)
{
	int i;
	char *eq, *name, *value;

	if (!args[1])
	{
		/* Print all aliases */
		for (i = 0; i < alias_count; i++)
			print_alias(&aliases[i]);
		return (0);
	}

	for (i = 1; args[i]; i++)
	{
		eq = _strchr(args[i], '=');
		if (eq)
		{
			/* Define or update alias */
			*eq = '\0';
			name = args[i];
			value = eq + 1;
			set_alias(name, value);
		}
		else
		{
			/* Print specific alias */
			alias_t *alias = find_alias(args[i]);
			if (alias)
				print_alias(alias);
		}
	}

	return (0);
}

/**
 * free_aliases - free all aliases
 */
void free_aliases(void)
{
	int i;

	for (i = 0; i < alias_count; i++)
	{
		free(aliases[i].name);
		free(aliases[i].value);
	}
	alias_count = 0;
}
