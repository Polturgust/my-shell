/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** alias.c - alias table and the alias / unalias builtins
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

static vec_t *table(void)
{
    static vec_t pairs = {NULL, 0, 0};

    if (!pairs.items)
        vec_init(&pairs);
    return &pairs;
}

static long alias_index(const char *name)
{
    vec_t *t = table();

    for (size_t i = 0; i < t->len; i += 2)
        if (strcmp(t->items[i], name) == 0)
            return (long)i;
    return -1;
}

const char *alias_get(const char *name)
{
    long idx = alias_index(name);

    return idx < 0 ? NULL : table()->items[idx + 1];
}

void alias_set(const char *name, const char *value)
{
    long idx = alias_index(name);

    if (idx >= 0) {
        free(table()->items[idx + 1]);
        table()->items[idx + 1] = xstrdup(value);
        return;
    }
    vec_push(table(), xstrdup(name));
    vec_push(table(), xstrdup(value));
}

void alias_unset(const char *name)
{
    vec_t *t = table();
    long idx = alias_index(name);

    if (idx < 0)
        return;
    free(t->items[idx]);
    free(t->items[idx + 1]);
    memmove(&t->items[idx], &t->items[idx + 2],
        sizeof(char *) * (t->len - idx - 1));
    t->len -= 2;
}

vec_t *alias_names(void)
{
    static vec_t names = {NULL, 0, 0};

    vec_free(&names);
    vec_init(&names);
    for (size_t i = 0; i < table()->len; i += 2)
        vec_push(&names, xstrdup(table()->items[i]));
    vec_sort(&names);
    return &names;
}

void alias_free(void)
{
    vec_free(table());
}
