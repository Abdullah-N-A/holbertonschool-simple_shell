#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

typedef unsigned char u8;

#define PROMPT "#cisfun$ "

typedef struct shell_s
{
    u8 *progname;
    char **envp;
    char *exit;
} shell_t;

shell_t *shell_init(int flags, u8 *progname, char **envp);
void shell_runtime(shell_t *shell);
void shell_free(shell_t *shell);
char **parse_args(char *line);
char *find_command(char *cmd, char **envp);

#endif /* SHELL_H */
