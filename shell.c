#include "shell.h"

/**
 * shell_init - initialize the shell structure
 * @flags: unused for now
 * @progname: name of the program
 * @envp: environment variables
 *
 * Return: pointer to shell_t structure
 */
shell_t *shell_init(int flags, u8 *progname, char **envp)
{
    shell_t *shell;

    (void)flags;

    shell = malloc(sizeof(shell_t));
    if (!shell)
        return NULL;

    shell->progname = progname;
    shell->envp = envp;
    shell->exit = NULL;

    return shell;
}

/**
 * parse_args - very simple parser (splits line by spaces)
 * @line: input command line
 *
 * Return: NULL-terminated array of arguments
 */
char **parse_args(char *line)
{
    char **args;
    int i;
    char *token, *line_copy;

    if (!line)
        return NULL;

    line_copy = strdup(line);
    if (!line_copy)
        return NULL;

    args = malloc(64 * sizeof(char *)); /* max 64 arguments */
    if (!args)
    {
        free(line_copy);
        return NULL;
    }

    token = strtok(line_copy, " \t\n");
    i = 0;
    while (token != NULL && i < 63)
    {
        args[i] = strdup(token);
        token = strtok(NULL, " \t\n");
        i++;
    }
    args[i] = NULL;
    free(line_copy);
    return args;
}

/**
 * find_command - find full path of command using PATH
 * @cmd: command name
 * @envp: environment variables
 *
 * Return: full path string (malloced) or NULL if not found
 */
char *find_command(char *cmd, char **envp)
{
    char *path_env, *path_copy, *dir, *full_path;
    int len, i;

    if (strchr(cmd, '/')) /* command already has /, use it directly */
        return strdup(cmd);

    path_env = NULL;
    for (i = 0; envp[i]; i++)
    {
        if (strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_env = envp[i] + 5;
            break;
        }
    }
    if (!path_env)
        return NULL;

    path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir)
    {
        len = strlen(dir) + strlen(cmd) + 2;
        full_path = malloc(len);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }
        snprintf(full_path, len, "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

/**
 * shell_runtime - main shell loop
 * @shell: pointer to shell_t
 */
void shell_runtime(shell_t *shell)
{
    char *line;
    char **args;
    size_t len;
    ssize_t read;
    pid_t pid;
    int status;
    int i;
    char *cmd_path;

    line = NULL;
    len = 0;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);

        read = getline(&line, &len, stdin);
        if (read == -1)
            break;

        args = parse_args(line);
        if (!args || !args[0])
        {
            free(args);
            continue;
        }

        cmd_path = find_command(args[0], shell->envp);
        if (!cmd_path)
        {
            perror(args[0]);
            for (i = 0; args[i]; i++)
                free(args[i]);
            free(args);
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror((char *)shell->progname);
            free(cmd_path);
            for (i = 0; args[i]; i++)
                free(args[i]);
            free(args);
            continue;
        }
        else if (pid == 0) /* child */
        {
            execve(cmd_path, args, shell->envp);
            perror(cmd_path);
            free(cmd_path);
            exit(EXIT_FAILURE);
        }
        else /* parent */
        {
            waitpid(pid, &status, 0);
        }

        free(cmd_path);
        for (i = 0; args[i]; i++)
            free(args[i]);
        free(args);
    }

    free(line);
}

/**
 * shell_free - free shell structure
 * @shell: pointer to shell_t
 */
void shell_free(shell_t *shell)
{
    free(shell);
}
