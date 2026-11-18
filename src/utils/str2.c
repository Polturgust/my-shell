/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** str2.c - small string helpers
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

void str_push(str_t *s, char c)
{
    str_appendn(s, &c, 1);
}

void str_append(str_t *s, const char *text)
{
    str_appendn(s, text, strlen(text));
}

void str_set(str_t *s, const char *text)
{
    s->len = 0;
    s->data[0] = '\0';
    str_append(s, text);
}

void str_free(str_t *s)
{
    free(s->data);
    s->data = NULL;
    s->len = 0;
    s->cap = 0;
}

int print_error(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vdprintf(2, fmt, ap);
    va_end(ap);
    sh()->error_raised = true;
    return 1;
}

int print_notice(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vdprintf(2, fmt, ap);
    va_end(ap);
    return 1;
}

bool should_abort(void)
{
    if (!sh()->abort_on_error || !sh()->error_raised)
        return false;
    sh()->do_exit = true;
    sh()->status = 1;
    return true;
}
