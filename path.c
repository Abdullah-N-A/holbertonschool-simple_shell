#include "shell.h"

char *find_path(char *cmd)
{
    char *path = getenv("PATH");
    char *path_copy, *dir;
    static char fullpath[1024];

    if (!path)
        return NULL;
    path_copy = strdup(path);
    dir = strtok(path_copy, ":");
    while (dir)
    {
        _strcpy(fullpath, dir);
        _strcat(fullpath, "/");
        _strcat(fullpath, cmd);
        if (access(fullpath, X_OK) == 0)
        {
            free(path_copy);
            return fullpath;
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}
