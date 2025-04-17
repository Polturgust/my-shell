/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** builtin_set_unset.c
*/
#include "variables.h"

int builtin_set(char **args, var_t **vars)
{
    char *equal_sign;

    if (!args[1]) {
        print_variables(*vars);
        return 0;
    }
    for (int i = 1; args[i]; i++) {
        equal_sign = strchr(args[i], '=');
        if (equal_sign) {
            *equal_sign = '\0';
            set_variable(vars, args[i], equal_sign + 1);
        } else
            fprintf(stderr, "set: invalid format: %s\n", args[i]);
    }
    return 0;
}

int builtin_unset(char **args, var_t **vars)
{
    if (!args[1]) {
        fprintf(stderr, "unset: not enough arguments\n");
        return 1;
    }
    for (int i = 1; args[i]; i++)
        unset_variable(vars, args[i]);
    return 0;
}
