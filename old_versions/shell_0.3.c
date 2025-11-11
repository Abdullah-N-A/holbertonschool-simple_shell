#include <stdio.h>      // printf, perror, dprintf
#include <stdlib.h>     // malloc, free, exit
#include <string.h>     // strtok, strcmp
#include <unistd.h>     // fork, execve, isatty, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // waitpid, WIFEXITED, WEXITSTATUS
#include <stddef.h>     // size_t

extern char **environ; // for execve

/* Function prototypes */
char *read_line(void);
char **parse_args(char *line);
void free_args(char **args);
char *find_in_path(char *cmd);
void display_prompt(void);
int run_command(char **argv_exec, char *progname);

/* Display shell prompt */
void display_prompt(void)
{
    write(STDOUT_FILENO, "$ ", 2);
}

/* Read a line from stdin */
char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    if (getline(&line, &bufsize, stdin) == -1)
    {
        free(line);
        return NULL;
    }
    /* Remove trailing newline */
    line[strcspn(line, "\n")] = '\0';
    return line;
}

/* Split line into arguments */
char **parse_args(char *line)
{
    char **argv = NULL;
    char *token;
    size_t argc = 0;

    token = strtok(line, " ");
    while (token)
    {
        argv = realloc(argv, sizeof(char *) * (argc + 2));
        argv[argc++] = strdup(token);
        token = strtok(NULL, " ");
    }
    if (argv)
        argv[argc] = NULL;
    return argv;
}

/* Free parsed arguments */
void free_args(char **args)
{
    size_t i = 0;
    if (!args)
        return;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}

/* Find command in PATH */
char *find_in_path(char *cmd)
{
    char *path = getenv("PATH");
    char *path_copy, *dir;
    char *full_path = NULL;
    size_t len;

    if (!path)
        return NULL;

    path_copy = strdup(path);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir)
    {
        len = strlen(dir) + 1 + strlen(cmd) + 1;
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

/* Run a command with arguments */
int run_command(char **argv_exec, char *progname)
{
    pid_t pid;
    int status;
    char *full_path;

    if (!argv_exec || !argv_exec[0])
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

    if (pid == 0) /* child */
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
    return 0;
}

/* Main shell loop */
int main(int argc, char **argv)
{
    char *line;
    char **argv_exec;
    int status;

    (void)argc;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            display_prompt();

        line = read_line();
        if (!line)
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        argv_exec = parse_args(line);
        free(line);

        if (!argv_exec)
            continue;

        status = run_command(argv_exec, argv[0]);
        free_args(argv_exec);
    }

    return 0;
}
