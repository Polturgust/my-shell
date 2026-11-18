/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** path.c - find a program in $PATH
*/

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "shell.h"

static bool is_program(const char *path)
{
    struct stat st;

    return stat(path, &st) == 0 && S_ISREG(st.st_mode)
        && access(path, X_OK) == 0;
}

char *find_command(const char *name)
{
    const char *path = env_get("PATH");
    char *dirs = NULL;
    char *dir = NULL;
    char *save = NULL;
    char *full = NULL;

    if (!name[0])
        return NULL;
    if (strchr(name, '/'))
        return access(name, F_OK) == 0 ? xstrdup(name) : NULL;
    dirs = xstrdup(path ? path : DEFAULT_PATH);
    for (dir = strtok_r(dirs, ":", &save); dir;
        dir = strtok_r(NULL, ":", &save)) {
        full = path_join(dir[0] ? dir : ".", name);
        if (is_program(full))
            break;
        free(full);
        full = NULL;
    }
    free(dirs);
    return full;
}
