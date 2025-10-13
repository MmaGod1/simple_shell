#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

int shell(char **args, char **av);
char *find_path(char *command);
char *_get_path(void);
void print_env(void);
char *_getline(void);
int handle_exit(char **args, char *line, int status, char **av);


/** built-in functions **/
char *_getenv(const char *name);
int _setenv(const char *name, const char *value, int overwrite);
int handle_setenv(char **args);
int handle_unsetenv(char **args);
int _unsetenv(const char *name);
void free_env(void);
int handle_cd(char **args);
int handle_builtin(char **args, int *status);


/** String helper functions **/
int _strlen(char *s);
char *_strcpy(char *dest, char *src);
char *_strcat(char *dest, char *src);
char *_strdup(char *str);
int _strcmp(char *s1, char *s2);
int is_delimiter(char c, const char *delim);
int _strtok(char *line, char **args, int max_args, const char *delim);
int _atoi(char *s);
int _strncmp(char *s1, char *s2, int n);
char *_strchr(const char *s, char c);
void _strncpy(char *dest, const char *src, size_t n);

/** Seoarators and Operators */
int parse_line(char *line, char **commands, char **operators);int execute_with_operators(char *line, char **av, int *status);


/** Alias Implementation functions **/
typedef struct alias_s
{
	char *name;
	char *value;
} alias_t;

alias_t *find_alias(const char *name);
int set_alias(const char *name, const char *value);
void print_alias(alias_t *alias);
int handle_alias(char **args);
void free_aliases(void);

#endif
