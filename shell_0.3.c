#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

/**
 * display_prompt - print shell prompt
 */
void display_prompt(void)
{
    write(STDOUT_FILENO, "#cisfun$ ", 9);
}

/**
 * read_line - read one line from stdin using getline
 * Return: malloc'ed buffer or NULL on EOF/error
 */
char *read_line(void)
{
    char *line = NULL;
    size_t n = 0;
    ssize_t r;

    r = getline(&line, &n, stdin);
    if (r == -1)
    {
        free(line);
        return NULL;
    }
    if (r > 0 && line[r - 1] == '\n')
        line[r - 1] = '\0';
    return line;
}

/**
 * parse_args - split a line into argv by spaces/tabs
 */
char **parse_args(char *line)
{
    char *tok;
    size_t cap = 8, argc = 0;
    char **argv_exec;
    size_t newcap;
    char **tmp;

    while (*line == ' ' || *line == '\t')
        line++;

    if (*line == '\0')
        return NULL;

    argv_exec = malloc(sizeof(char *) * cap);
    if (!argv_exec)
        return NULL;

    tok = strtok(line, " \t");
    while (tok)
    {
        if (argc + 1 >= cap)
        {
            newcap = cap * 2;
            tmp = realloc(argv_exec, sizeof(char *) * newcap);
            if (!tmp)
            {
                free(argv_exec);
                return NULL;
            }
            argv_exec = tmp;
            cap = newcap;
        }
        argv_exec[argc++] = tok;
        tok = strtok(NULL, " \t");
    }
    argv_exec[argc] = NULL;

    if (argc == 0)
    {
        free(argv_exec);
        return NULL;
    }
    return argv_exec;
}

/**
 * free_args - free argv array (tokens are inside line buffer)
 */
void free_args(char **argv_exec)
{
    free(argv_exec);
}

/**
 * find_in_path - search for a command in PATH using environ
 */
char *find_in_path(char *cmd)
{
    char *path_var = NULL, *path_copy, *dir;
    char *full_path = NULL;
    int i;

    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return strdup(cmd);
        return NULL;
    }

    for (i = 0; environ[i]; i++)
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            path_var = environ[i] + 5;
            break;
        }
    }
    if (!path_var)
        return NULL;

    path_copy = strdup(path_var);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir)
    {
        size_t len = strlen(dir) + strlen(cmd) + 2;
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
        full_path = NULL;
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

/**
 * run_command - execute a command with arguments
 */
int run_command(char **argv_exec, const char *progname)
{
    pid_t pid;
    char *full_path;
    int status;

    if (!argv_exec || !argv_exec[0] || argv_exec[0][0] == '\0')
        return 0;

    full_path = find_in_path(argv_exec[0]);
    if (!full_path)
    {
        dprintf(STDERR_FILENO, "%s: %s: not found\n", progname, argv_exec[0]);
        return 127;
    }

    pid = fork();
    if (pid == -1)
    {
        perror(progname);
        free(full_path);
        return 1;
    }

    if (pid == 0)
    {
        execve(full_path, argv_exec, environ);
        perror(progname);
        free(full_path);
        _exit(127);
    }

    free(full_path);
    if (waitpid(pid, &status, 0) == -1)
    {
        perror(progname);
        return 1;
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return 1;
}

/**
 * main - simple shell
 */
int main(int argc, char **argv)
{
    int status = 0;
    char *line;
    char **argv_exec;

    (void)argc;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            display_prompt();

        line = read_line();
        if (!line)
        {
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        argv_exec = parse_args(line);
        if (argv_exec)
        {
            status = run_command(argv_exec, argv[0]);
            free_args(argv_exec);
        }

        free(line);
    }

    return status;
}
