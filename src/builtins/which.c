/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** which.c - which, where
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "shell.h"

static int which_one(const char *name)
{
    char *path = NULL;

    if (alias_get(name)) {
        dprintf(1, "%s: \t aliased to %s\n", name, alias_get(name));
        return 0;
    }
    if (builtin_find(name)) {
        dprintf(1, "%s: shell built-in command.\n", name);
        return 0;
    }
    path = find_command(name);
    if (!path)
        return print_notice("%s: Command not found.\n", name);
    dprintf(1, "%s\n", path);
    free(path);
    return 0;
}

int builtin_which(char **argv)
{
    int status = 0;

    if (!argv[1])
        return print_error("which: Too few arguments.\n");
    for (size_t i = 1; argv[i]; i++)
        status |= which_one(argv[i]);
    return status;
}

static int where_path(const char *name)
{
    char *dirs = xstrdup(env_get("PATH") ? env_get("PATH") : DEFAULT_PATH);
    char *save = NULL;
    char *full = NULL;
    int found = 0;

    for (char *d = strtok_r(dirs, ":", &save); d;
        d = strtok_r(NULL, ":", &save)) {
        full = path_join(d, name);
        if (access(full, X_OK) == 0 && ++found)
            dprintf(1, "%s\n", full);
        free(full);
    }
    free(dirs);
    return found;
}

int builtin_where(char **argv)
{
    int status = 0;
    int found = 0;

    if (!argv[1])
        return print_error("where: Too few arguments.\n");
    for (size_t i = 1; argv[i]; i++) {
        found = 0;
        if (alias_get(argv[i]) && ++found)
            dprintf(1, "%s is aliased to %s\n", argv[i], alias_get(argv[i]));
        if (builtin_find(argv[i]) && ++found)
            dprintf(1, "%s is a shell built-in\n", argv[i]);
        found += where_path(argv[i]);
        status |= found == 0;
    }
    return status;
}
