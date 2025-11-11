#include "shell.h"

/* initialize shell struct */
shell_t *shell_init(int flags, u8 *progname, char **envp)
{
    shell_t *shell;

    shell = malloc(sizeof(shell_t));
    if (!shell)
        return NULL;

    shell->progname = progname;
    shell->envp = envp;
    shell->exit = NULL;

    (void)flags;
    return shell;
}

/* free shell struct */
void shell_free(shell_t *shell)
{
    if (!shell)
        return;
    free(shell);
}

/* simple runtime loop */
shell_t *shell_runtime(shell_t *shell)
{
    char *line;
    ssize_t nread;
    size_t n;

    if (!shell)
        return NULL;

    while (1)
    {
        n = 0;
        printf("%s", PROMPT);
        nread = getline(&line, &n, stdin);
        if (nread == -1)
        {
            printf("\n");
            break;
        }

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* exit shell if command is "exit" */
        if (strcmp(line, "exit") == 0)
        {
            if (shell->exit)
                *(shell->exit) = 0;
            free(line);
            break;
        }

        if (fork() == 0)
        {
            char *args[2];
            args[0] = line;
            args[1] = NULL;
            if (execvp(args[0], args) == -1)
            {
                perror((const char *)shell->progname);
                _exit(1);
            }
        }
        else
        {
            wait(NULL);
        }

        free(line);
        line = NULL;
    }

    return shell;
}
