#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

extern char **environ;

typedef unsigned char u8;

typedef struct shell_s
{
    u8 *progname;
    char **envp;
    int *exit;
} shell_t;

shell_t *shell_init(int flags, u8 *progname, char **envp);
char **parse_args(char *line);
char *find_command(char *cmd, char **envp);
void shell_runtime(shell_t *shell);
void shell_free(shell_t *shell);

#define PROMPT ":) "

#endif /* SHELL_H */
