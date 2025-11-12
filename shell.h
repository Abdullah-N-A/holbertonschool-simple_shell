#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* Prototypes */
char **split_line(char *line, const char *delim);
int execute(char **args);
int builtin_exit(char **args);
int builtin_env(char **args);
char *_strcpy(char *dest, const char *src);
char *_strcat(char *dest, const char *src);

#endif
