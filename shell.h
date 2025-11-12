#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

extern char **environ;

char **split_line(char *line, const char *delim);
int _strlen(char *s);
char *_strcpy(char *dest, char *src);
char *_strcat(char *dest, char *src);
char *find_path(char *cmd);
int execute(char **args);
int builtin_exit(char **args);
int builtin_env(char **args);
char *get_path(void);

#endif
