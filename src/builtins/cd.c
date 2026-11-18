/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** cd.c - change directory, keep PWD / OLDPWD / $cwd up to date
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

static const char *cd_target(char **argv)
{
    const char *home = env_get("HOME");

    if (!argv[1] || strcmp(argv[1], "~") == 0) {
        if (!home)
            print_error("cd: No home directory.\n");
        return home;
    }
    if (strcmp(argv[1], "-") == 0) {
        if (!env_get("OLDPWD"))
            print_error(": No such file or directory.\n");
        return env_get("OLDPWD");
    }
    return argv[1];
}

static void update_pwd(const char *old)
{
    char *cwd = getcwd(NULL, 0);

    if (old)
        env_set("OLDPWD", old);
    if (cwd) {
        env_set("PWD", cwd);
        set_variable(&sh()->vars, "cwd", cwd);
    }
    free(cwd);
}

int builtin_cd(char **argv)
{
    const char *target = NULL;
    char *old = NULL;

    if (argv[1] && argv[2])
        return print_error("cd: Too many arguments.\n");
    target = cd_target(argv);
    if (!target)
        return 1;
    old = getcwd(NULL, 0);
    if (chdir(target) < 0) {
        print_error("%s: %s.\n", target,
            errno == ENOTDIR ? "Not a directory" : strerror(errno));
        free(old);
        return 1;
    }
    update_pwd(old);
    free(old);
    run_alias_hook("cwdcmd");
    return 0;
}
