#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

typedef unsigned char u8;

/* Shell struct */
typedef struct shell_s
{
    u8 *progname;
    char **envp;
    int *exit;
} shell_t;

/* Shell functions */
shell_t *shell_init(int flags, u8 *progname, char **envp);
void shell_runtime(shell_t *shell);
void shell_free(shell_t *shell);
char **parse_args(char *line);

#define PROMPT "#cisfun$ "

#endif /* SHELL_H */
