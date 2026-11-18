/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** str.c - growable strings
*/

#include <stdlib.h>
#include <string.h>
#include "shell.h"

static void str_reserve(str_t *s, size_t extra)
{
    if (s->len + extra + 1 <= s->cap)
        return;
    while (s->len + extra + 1 > s->cap)
        s->cap = s->cap ? s->cap * 2 : 32;
    s->data = xrealloc(s->data, s->cap);
}

void str_init(str_t *s)
{
    s->data = NULL;
    s->len = 0;
    s->cap = 0;
    str_reserve(s, 0);
    s->data[0] = '\0';
}

void str_appendn(str_t *s, const char *text, size_t n)
{
    str_reserve(s, n);
    memcpy(s->data + s->len, text, n);
    s->len += n;
    s->data[s->len] = '\0';
}

void str_insert(str_t *s, size_t pos, const char *text, size_t n)
{
    str_reserve(s, n);
    memmove(s->data + pos + n, s->data + pos, s->len - pos + 1);
    memcpy(s->data + pos, text, n);
    s->len += n;
}

void str_erase(str_t *s, size_t pos, size_t n)
{
    if (pos >= s->len)
        return;
    if (pos + n > s->len)
        n = s->len - pos;
    memmove(s->data + pos, s->data + pos + n, s->len - pos - n + 1);
    s->len -= n;
}

char *str_take(str_t *s)
{
    char *data = s->data;

    s->data = NULL;
    s->len = 0;
    s->cap = 0;
    return data;
}
