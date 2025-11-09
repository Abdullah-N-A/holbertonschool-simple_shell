#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

/* Environment variable */
extern char **environ;

/* Function prototypes */
void display_prompt(void);
char *read_line(void);
char **parse_line(char *line);
int execute_command(char **args, char *argv0);
char *find_path(char *command);
void free_array(char **array);

#endif /* MAIN_H */
