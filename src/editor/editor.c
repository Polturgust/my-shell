/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** editor.c - read one line from the terminal with editing
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "editor.h"

static void editor_init(editor_t *e, const char *prompt)
{
    memset(e, 0, sizeof(editor_t));
    str_init(&e->buf);
    str_init(&e->kill);
    str_init(&e->query);
    e->prompt = prompt;
    e->hist_index = -1;
}

static void editor_free(editor_t *e)
{
    str_free(&e->kill);
    str_free(&e->query);
    free(e->saved);
    free(e->before_search);
}

static void insert_utf8(editor_t *e, unsigned char c)
{
    char seq[4] = {(char)c, 0, 0, 0};
    size_t len = c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1;

    for (size_t i = 1; i < len; i++)
        if (read(STDIN_FILENO, &seq[i], 1) != 1)
            len = i;
    edit_insert(e, seq, len);
}

static key_result_t handle(editor_t *e, unsigned char c)
{
    key_result_t res = K_CONTINUE;

    if (e->searching && search_key(e, c, &res))
        return res;
    if (c >= 32 && c != 127) {
        insert_utf8(e, c);
        return K_CONTINUE;
    }
    return editor_key(e, c);
}

char *editor_read(const char *prompt, bool *eof)
{
    editor_t e;
    unsigned char c = 0;
    key_result_t res = K_CONTINUE;

    editor_init(&e, prompt);
    term_raw();
    editor_refresh(&e);
    while (res == K_CONTINUE) {
        if (read(STDIN_FILENO, &c, 1) != 1) {
            res = K_EOF;
            break;
        }
        res = handle(&e, c);
        if (res == K_ACCEPT)
            e.pos = e.buf.len;
        if (!e.cancelled)
            editor_refresh(&e);
    }
    if (res == K_ACCEPT && !e.cancelled)
        write(STDOUT_FILENO, "\r\n", 2);
    term_restore();
    *eof = res == K_EOF;
    editor_free(&e);
    if (*eof) {
        str_free(&e.buf);
        return NULL;
    }
    return str_take(&e.buf);
}
