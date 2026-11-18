/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** render.c - redraw the prompt and the line, wrapping on narrow terminals
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "editor.h"

size_t display_width(const char *s, size_t n)
{
    size_t width = 0;

    for (size_t i = 0; i < n && s[i]; i++) {
        if (s[i] == '\033') {
            while (i < n && s[i] && !((s[i] >= 'a' && s[i] <= 'z')
                || (s[i] >= 'A' && s[i] <= 'Z')))
                i++;
            continue;
        }
        if (((unsigned char)s[i] & 0xC0) != 0x80)
            width++;
    }
    return width;
}

static void append_fmt(str_t *out, const char *fmt, size_t value)
{
    char tmp[32];

    snprintf(tmp, sizeof(tmp), fmt, value);
    str_append(out, tmp);
}

static const char *current_prompt(editor_t *e, str_t *tmp)
{
    if (!e->searching)
        return e->prompt;
    str_set(tmp, "(reverse-i-search)`");
    str_append(tmp, e->query.data);
    str_append(tmp, "': ");
    return tmp->data;
}

static void place_cursor(editor_t *e, str_t *out, size_t pw, size_t total)
{
    size_t cols = (size_t)term_width();
    size_t cur = pw + display_width(e->buf.data, e->pos);
    size_t end_row = total / cols;
    size_t row = cur / cols;

    if (total > 0 && total % cols == 0)
        str_append(out, "\r\n");
    if (end_row > row)
        append_fmt(out, "\033[%zuA", end_row - row);
    str_append(out, "\r");
    if (cur % cols)
        append_fmt(out, "\033[%zuC", cur % cols);
    e->cursor_row = row;
}

void editor_refresh(editor_t *e)
{
    str_t out;
    str_t tmp;
    const char *prompt = NULL;
    size_t pw = 0;

    str_init(&out);
    str_init(&tmp);
    prompt = current_prompt(e, &tmp);
    pw = display_width(prompt, strlen(prompt));
    if (e->cursor_row > 0)
        append_fmt(&out, "\033[%zuA", e->cursor_row);
    str_append(&out, "\r");
    str_append(&out, prompt);
    str_append(&out, e->buf.data);
    str_append(&out, "\033[J");
    place_cursor(e, &out, pw, pw + display_width(e->buf.data, e->buf.len));
    write(STDOUT_FILENO, out.data, out.len);
    str_free(&out);
    str_free(&tmp);
}

void editor_redraw_all(editor_t *e)
{
    print_prompt_header();
    e->cursor_row = 0;
    editor_refresh(e);
}
