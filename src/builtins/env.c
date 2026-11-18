/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** env.c - env, setenv, unsetenv
*/

#include <stdio.h>
#include "shell.h"

int builtin_env(char **argv)
{
    char **env = env_array();

    (void)argv;
    for (size_t i = 0; env[i]; i++)
        dprintf(1, "%s\n", env[i]);
    return 0;
}

static int check_name(const char *name)
{
    if (!is_name_char(name[0], true))
        return print_error("setenv: Variable name must begin with a letter.\n");
    for (size_t i = 0; name[i]; i++)
        if (!is_name_char(name[i], false))
            return print_error("setenv: Variable name must contain "
                "alphanumeric characters.\n");
    return 0;
}

int builtin_setenv(char **argv)
{
    if (!argv[1])
        return builtin_env(argv);
    if (argv[2] && argv[3])
        return print_error("setenv: Too many arguments.\n");
    if (check_name(argv[1]))
        return 1;
    env_set(argv[1], argv[2] ? argv[2] : "");
    return 0;
}

int builtin_unsetenv(char **argv)
{
    if (!argv[1])
        return print_error("unsetenv: Too few arguments.\n");
    for (size_t i = 1; argv[i]; i++)
        env_unset(argv[i]);
    return 0;
}
