/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** table.c - name -> builtin function
*/

#include <stdio.h>
#include <string.h>
#include "shell.h"

typedef struct builtin_s {
    const char *name;
    builtin_fn_t fn;
} builtin_t;

static const builtin_t BUILTINS[] = {
    {"alias", builtin_alias}, {"bg", builtin_bg},
    {"builtins", builtin_builtins}, {"cd", builtin_cd},
    {"chdir", builtin_cd}, {"echo", builtin_echo}, {"env", builtin_env},
    {"exit", builtin_exit}, {"fg", builtin_fg}, {"history", builtin_history},
    {"jobs", builtin_jobs}, {"repeat", builtin_repeat}, {"set", builtin_set},
    {"setenv", builtin_setenv}, {"source", builtin_source},
    {"unalias", builtin_unalias}, {"unset", builtin_unset},
    {"unsetenv", builtin_unsetenv}, {"where", builtin_where},
    {"which", builtin_which}, {NULL, NULL}
};

builtin_fn_t builtin_find(const char *name)
{
    for (size_t i = 0; BUILTINS[i].name; i++)
        if (strcmp(BUILTINS[i].name, name) == 0)
            return BUILTINS[i].fn;
    return NULL;
}

const char **builtin_names(void)
{
    static const char *names[sizeof(BUILTINS) / sizeof(BUILTINS[0])];

    for (size_t i = 0; BUILTINS[i].name; i++)
        names[i] = BUILTINS[i].name;
    return names;
}

int builtin_builtins(char **argv)
{
    const char **names = builtin_names();

    (void)argv;
    for (size_t i = 0; names[i]; i++)
        dprintf(1, "%s%s", names[i], names[i + 1] ? "\t" : "\n");
    return 0;
}
