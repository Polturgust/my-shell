/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** keys.c - what every key does
**
** Ctrl+A/E start/end, Ctrl+B/F left/right, Ctrl+P/N history,
** Ctrl+K/U/W kill, Ctrl+Y yank, Ctrl+L clear, Ctrl+R search, Tab complete
*/

#include <unistd.h>
#include "editor.h"

static key_result_t control_key(editor_t *e, unsigned char c)
{
    switch (c) {
    case 1:
        e->pos = 0;
        break;
    case 2:
        edit_left(e);
        break;
    case 5:
        e->pos = e->buf.len;
        break;
    case 6:
        edit_right(e);
        break;
    case 11:
        edit_kill(e, e->pos, e->buf.len);
        break;
    case 14:
        hist_next(e);
        break;
    case 16:
        hist_prev(e);
        break;
    default:
        return K_CONTINUE;
    }
    return K_CONTINUE;
}

static key_result_t control_key2(editor_t *e, unsigned char c)
{
    size_t end = e->pos;

    switch (c) {
    case 12:
        write(STDOUT_FILENO, "\033[H\033[2J", 7);
        editor_redraw_all(e);
        break;
    case 18:
        search_start(e);
        break;
    case 21:
        edit_kill(e, 0, e->pos);
        break;
    case 23:
        edit_word_left(e);
        edit_kill(e, e->pos, end);
        break;
    case 25:
        edit_yank(e);
        break;
    default:
        return control_key(e, c);
    }
    return K_CONTINUE;
}

static key_result_t cancel_line(editor_t *e)
{
    e->pos = e->buf.len;
    editor_refresh(e);
    write(STDOUT_FILENO, "^C\r\n", 4);
    edit_set(e, "");
    e->cancelled = true;
    set_status(1);
    return K_ACCEPT;
}

key_result_t editor_key(editor_t *e, unsigned char c)
{
    if (c == '\r' || c == '\n')
        return K_ACCEPT;
    if (c == 3)
        return cancel_line(e);
    if (c == 4 && e->buf.len == 0)
        return K_EOF;
    if (c == 4)
        edit_delete(e);
    if (c == 127 || c == 8)
        edit_backspace(e);
    if (c == '\t')
        complete(e);
    if (c == 27)
        return editor_escape(e);
    if (c < 32 && c != 4 && c != 8 && c != '\t')
        return control_key2(e, c);
    return K_CONTINUE;
}
