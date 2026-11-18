/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** history_nav.c - up / down arrows and Ctrl+R reverse search
*/

#include <stdlib.h>
#include <string.h>
#include "editor.h"

void hist_prev(editor_t *e)
{
    size_t len = history_len();

    if ((size_t)(e->hist_index + 1) >= len)
        return;
    if (e->hist_index < 0) {
        free(e->saved);
        e->saved = xstrdup(e->buf.data);
    }
    e->hist_index++;
    edit_set(e, history_at(len - 1 - e->hist_index)->cmd);
}

void hist_next(editor_t *e)
{
    size_t len = history_len();

    if (e->hist_index < 0)
        return;
    e->hist_index--;
    if (e->hist_index < 0)
        edit_set(e, e->saved ? e->saved : "");
    else
        edit_set(e, history_at(len - 1 - e->hist_index)->cmd);
}

static void search_from(editor_t *e, long from)
{
    const char *found = NULL;

    for (long i = from; i >= 0 && e->query.len; i--) {
        found = strstr(history_at(i)->cmd, e->query.data);
        if (found) {
            e->match = i;
            edit_set(e, history_at(i)->cmd);
            e->pos = found - history_at(i)->cmd;
            return;
        }
    }
}

void search_start(editor_t *e)
{
    e->searching = true;
    str_set(&e->query, "");
    e->match = (long)history_len();
    free(e->before_search);
    e->before_search = xstrdup(e->buf.data);
}

bool search_key(editor_t *e, unsigned char c, key_result_t *res)
{
    *res = K_CONTINUE;
    if (c == 18) {
        search_from(e, e->match - 1);
        return true;
    }
    if (c == 127 || c == 8) {
        if (e->query.len)
            e->query.data[--e->query.len] = '\0';
        search_from(e, (long)history_len() - 1);
        return true;
    }
    if (c == 7 || c == 3) {
        e->searching = false;
        edit_set(e, e->before_search);
        return true;
    }
    if (c >= 32 && c != 127) {
        str_push(&e->query, (char)c);
        search_from(e, e->match < (long)history_len() ? e->match
            : (long)history_len() - 1);
        return true;
    }
    e->searching = false;
    return false;
}
