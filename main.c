#include "shell.h"




/**
 * main - function
 * @argc: int
 * @argv: char ptr ptr
 * @envp: char ptr ptr
 *
 * Return: int
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
