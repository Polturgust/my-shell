/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** edit2.c - deleting, killing and yanking
*/

#include <string.h>
#include "editor.h"

void edit_backspace(editor_t *e)
{
    size_t end = e->pos;

    edit_left(e);
    str_erase(&e->buf, e->pos, end - e->pos);
}

void edit_delete(editor_t *e)
{
    size_t start = e->pos;

    edit_right(e);
    str_erase(&e->buf, start, e->pos - start);
    e->pos = start;
}

void edit_kill(editor_t *e, size_t from, size_t to)
{
    if (from >= to)
        return;
    str_set(&e->kill, "");
    str_appendn(&e->kill, e->buf.data + from, to - from);
    str_erase(&e->buf, from, to - from);
    e->pos = from;
}

void edit_yank(editor_t *e)
{
    edit_insert(e, e->kill.data, e->kill.len);
}

void edit_set(editor_t *e, const char *text)
{
    str_set(&e->buf, text);
    e->pos = e->buf.len;
}
