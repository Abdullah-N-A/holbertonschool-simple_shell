#include "shell.h"

char *get_path(void)
{
    int i = 0;

    while (environ[i])
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
            return environ[i] + 5;
        i++;
    }
    return NULL;
}

char *find_path(char *cmd)
{
    char *path_env = get_path();
    char *path_copy, *dir;
    static char cmd_path[1024];

    if (!path_env)
        return NULL;

    path_copy = strdup(path_env);
    dir = strtok(path_copy, ":");
    while (dir)
    {
        _strcpy(cmd_path, dir);
        _strcat(cmd_path, "/");
        _strcat(cmd_path, cmd);

        if (access(cmd_path, X_OK) == 0)
        {
            free(path_copy);
            return cmd_path;
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}
