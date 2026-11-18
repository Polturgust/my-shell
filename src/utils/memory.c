/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** memory.c - allocation helpers that never return NULL
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

void *xmalloc(size_t size)
{
    void *ptr = malloc(size ? size : 1);

    if (!ptr) {
        perror(SHELL_NAME);
        exit(1);
    }
    return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size ? size : 1);

    if (!new_ptr) {
        perror(SHELL_NAME);
        exit(1);
    }
    return new_ptr;
}

char *xstrdup(const char *s)
{
    return xstrndup(s, strlen(s));
}

char *xstrndup(const char *s, size_t n)
{
    char *copy = xmalloc(n + 1);

    memcpy(copy, s, n);
    copy[n] = '\0';
    return copy;
}
