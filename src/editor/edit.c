/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** edit.c - cursor movement and text changes (UTF-8 aware)
*/

#include <string.h>
#include "editor.h"

static bool is_cont(editor_t *e, size_t pos)
{
    return pos < e->buf.len && ((unsigned char)e->buf.data[pos] & 0xC0) == 0x80;
}

void edit_insert(editor_t *e, const char *text, size_t n)
{
    str_insert(&e->buf, e->pos, text, n);
    e->pos += n;
}

void edit_left(editor_t *e)
{
    if (e->pos == 0)
        return;
    e->pos--;
    while (e->pos > 0 && is_cont(e, e->pos))
        e->pos--;
}

void edit_right(editor_t *e)
{
    if (e->pos >= e->buf.len)
        return;
    e->pos++;
    while (is_cont(e, e->pos))
        e->pos++;
}

void edit_word_left(editor_t *e)
{
    while (e->pos > 0 && strchr(" \t/", e->buf.data[e->pos - 1]))
        e->pos--;
    while (e->pos > 0 && !strchr(" \t/", e->buf.data[e->pos - 1]))
        e->pos--;
}

void edit_word_right(editor_t *e)
{
    while (e->pos < e->buf.len && strchr(" \t/", e->buf.data[e->pos]))
        e->pos++;
    while (e->pos < e->buf.len && !strchr(" \t/", e->buf.data[e->pos]))
        e->pos++;
}
