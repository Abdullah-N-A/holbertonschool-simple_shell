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
int run_command(const char *cmd, const char *argv0);

#endif /* MAIN_H */
