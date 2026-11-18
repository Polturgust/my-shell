/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** escape.c - arrow keys, Home/End/Delete, Ctrl/Alt + arrows, Alt+B/F/D
*/

#include <string.h>
#include <unistd.h>
#include "editor.h"

static int read_byte(void)
{
    unsigned char c = 0;

    if (read(STDIN_FILENO, &c, 1) != 1)
        return -1;
    return c;
}

static void alt_key(editor_t *e, int c)
{
    size_t start = e->pos;

    if (c == 'b')
        edit_word_left(e);
    if (c == 'f')
        edit_word_right(e);
    if (c == 'd') {
        edit_word_right(e);
        edit_kill(e, start, e->pos);
    }
    if (c == 127 || c == 8) {
        edit_word_left(e);
        edit_kill(e, e->pos, start);
    }
}

static void final_key(editor_t *e, int c, bool modified)
{
    if (c == 'A')
        hist_prev(e);
    if (c == 'B')
        hist_next(e);
    if (c == 'C')
        modified ? edit_word_right(e) : edit_right(e);
    if (c == 'D')
        modified ? edit_word_left(e) : edit_left(e);
    if (c == 'H')
        e->pos = 0;
    if (c == 'F')
        e->pos = e->buf.len;
}

static void tilde_key(editor_t *e, const char *params)
{
    int code = params[0] ? (params[0] - '0') : 0;

    if (code == 1 || code == 7)
        e->pos = 0;
    if (code == 4 || code == 8)
        e->pos = e->buf.len;
    if (code == 3)
        edit_delete(e);
}

key_result_t editor_escape(editor_t *e)
{
    char params[16];
    size_t n = 0;
    int c = read_byte();

    if (c != '[' && c != 'O') {
        alt_key(e, c);
        return K_CONTINUE;
    }
    memset(params, 0, sizeof(params));
    c = read_byte();
    while (c >= 0 && ((c >= '0' && c <= '9') || c == ';')) {
        if (n + 1 < sizeof(params))
            params[n++] = (char)c;
        c = read_byte();
    }
    if (c == '~')
        tilde_key(e, params);
    else if (c > 0)
        final_key(e, c, strchr(params, ';') != NULL);
    return K_CONTINUE;
}
