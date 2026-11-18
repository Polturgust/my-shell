/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** alias2.c - alias / unalias builtins and alias hooks (precmd, cwdcmd)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

static void list_aliases(void)
{
    vec_t *names = alias_names();

    for (size_t i = 0; i < names->len; i++)
        dprintf(1, "%s\t%s\n", names->items[i], alias_get(names->items[i]));
}

static int define_assignment(const char *arg)
{
    const char *eq = strchr(arg, '=');
    char *name = xstrndup(arg, eq - arg);

    alias_set(name, eq + 1);
    free(name);
    return 0;
}

int builtin_alias(char **argv)
{
    str_t value;

    if (!argv[1]) {
        list_aliases();
        return 0;
    }
    if (!argv[2] && strchr(argv[1], '=') && argv[1][0] != '=')
        return define_assignment(argv[1]);
    if (!argv[2]) {
        if (alias_get(argv[1]))
            dprintf(1, "%s\n", alias_get(argv[1]));
        return 0;
    }
    if (strcmp(argv[1], "alias") == 0 || strcmp(argv[1], "unalias") == 0)
        return print_error("%s: Too dangerous to alias that.\n", argv[1]);
    str_init(&value);
    for (size_t i = 2; argv[i]; i++) {
        str_append(&value, i > 2 ? " " : "");
        str_append(&value, argv[i]);
    }
    alias_set(argv[1], value.data);
    str_free(&value);
    return 0;
}

int builtin_unalias(char **argv)
{
    if (!argv[1])
        return print_error("unalias: Too few arguments.\n");
    for (size_t i = 1; argv[i]; i++)
        alias_unset(argv[i]);
    return 0;
}

void run_alias_hook(const char *name)
{
    const char *value = alias_get(name);
    char *copy = NULL;
    int status = sh()->status;

    if (!value || sh()->in_child)
        return;
    copy = xstrdup(value);
    run_line(copy);
    free(copy);
    set_status(status);
}
