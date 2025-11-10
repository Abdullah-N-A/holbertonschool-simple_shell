#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

void display_prompt(void);
char *read_line(void);
char **parse_args(char *line);
void free_args(char **argv_exec);
int run_command(char **argv_exec, const char *progname);

#endif /* MAIN_H */
