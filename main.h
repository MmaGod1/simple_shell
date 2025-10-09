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
int handle_setenv(char **args);
int handle_unsetenv(char **args);
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

#endif
