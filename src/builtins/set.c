/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** set.c - shell variables: set, unset
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

static void list_variables(void)
{
    vec_t names;

    vec_init(&names);
    for (var_t *v = sh()->vars; v; v = v->next)
        vec_push(&names, xstrdup(v->key));
    vec_sort(&names);
    for (size_t i = 0; i < names.len; i++)
        dprintf(1, "%s\t%s\n", names.items[i],
            find_variable(sh()->vars, names.items[i])->value);
    vec_free(&names);
}

static int assign(const char *name, const char *value)
{
    if (!is_name_char(name[0], true))
        return print_error("set: Variable name must begin with a letter.\n");
    for (size_t i = 0; name[i]; i++)
        if (!is_name_char(name[i], false))
            return print_error("set: Variable name must contain "
                "alphanumeric characters.\n");
    set_variable(&sh()->vars, name, value);
    return 0;
}

static int set_one(char **argv, size_t *i)
{
    char *eq = strchr(argv[*i], '=');
    char *name = NULL;
    int ret = 0;

    if (!eq && argv[*i + 1] && strcmp(argv[*i + 1], "=") == 0) {
        ret = assign(argv[*i], argv[*i + 2] ? argv[*i + 2] : "");
        *i += argv[*i + 2] ? 3 : 2;
        return ret;
    }
    if (!eq) {
        (*i)++;
        return assign(argv[*i - 1], "");
    }
    name = xstrndup(argv[*i], eq - argv[*i]);
    if (!eq[1] && argv[*i + 1]) {
        ret = assign(name, argv[*i + 1]);
        *i += 2;
    } else {
        ret = assign(name, eq + 1);
        (*i)++;
    }
    free(name);
    return ret;
}

int builtin_set(char **argv)
{
    size_t i = 1;

    if (!argv[1]) {
        list_variables();
        return 0;
    }
    while (argv[i])
        if (set_one(argv, &i))
            return 1;
    return 0;
}

int builtin_unset(char **argv)
{
    if (!argv[1])
        return print_error("unset: Too few arguments.\n");
    for (size_t i = 1; argv[i]; i++)
        unset_variable(&sh()->vars, argv[i]);
    return 0;
}
