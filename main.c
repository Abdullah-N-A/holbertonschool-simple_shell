#include "shell.h"

/**
 * main - entry point for simple shell
 * @argc: argument count
 * @argv: argument vector
 * @envp: environment variables
 *
 * Return: exit status
 */
int main(int argc, char **argv, char **envp)
{
    int exit_status;
    shell_t *shell;

    exit_status = 0;
    shell = shell_init(0, (u8 *)argv[0], envp);
    if (!shell)
        return 1;

    shell->exit = &exit_status;
    shell_runtime(shell);
    shell_free(shell);

    (void)argc;
    return exit_status;
}
