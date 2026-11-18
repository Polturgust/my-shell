/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** vec.c - NULL-terminated string vectors
*/

#include <stdlib.h>
#include <string.h>
#include "shell.h"

void vec_init(vec_t *v)
{
    v->cap = 8;
    v->len = 0;
    v->items = xmalloc(sizeof(char *) * v->cap);
    v->items[0] = NULL;
}

void vec_push(vec_t *v, char *item)
{
    if (v->len + 2 > v->cap) {
        v->cap *= 2;
        v->items = xrealloc(v->items, sizeof(char *) * v->cap);
    }
    v->items[v->len] = item;
    v->len++;
    v->items[v->len] = NULL;
}

void vec_free(vec_t *v)
{
    if (!v->items)
        return;
    for (size_t i = 0; i < v->len; i++)
        free(v->items[i]);
    free(v->items);
    v->items = NULL;
    v->len = 0;
    v->cap = 0;
}

static int cmp_str(const void *a, const void *b)
{
    return strcmp(*(char *const *)a, *(char *const *)b);
}

void vec_sort(vec_t *v)
{
    qsort(v->items, v->len, sizeof(char *), cmp_str);
}
