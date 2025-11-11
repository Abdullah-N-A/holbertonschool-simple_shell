#include "shell.h"
#include <stdio.h>

int main(int argc, char **argv, char **envp)
{
	shell_t *shell;

	(void)argc;

	shell = shell_init(0, (u8 *)argv[0], envp);
	if (!shell)
		return 1;

	shell_runtime(shell);
	shell_free(shell);

	return 0;
}
