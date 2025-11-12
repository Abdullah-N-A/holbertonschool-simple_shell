#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

extern char **environ;
extern int last_status;

/* Main shell functions */
char **split_line(char *line, const char *delim);
int execute(char **args);
int process_command(char **command_args);

/* Built-in commands */
int builtin_exit(char **args);
int builtin_env(char **args);
int check_builtin(char **args);

/* Path and environment utilities */
char *get_env_value(char *name);
char *resolve_path(char *command);

/* String conversion utilities */
int str_to_int(char *str);
int validate_number(char *str);

#endif
