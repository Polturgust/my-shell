/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** complete.c - Tab: complete the word under the cursor, list choices
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "editor.h"

#define SPECIAL " \t'\"\\$`;&|<>()*?[]#!"

static size_t word_start(editor_t *e, bool *command)
{
    size_t start = 0;
    size_t i = 0;
    char quote = 0;

    for (i = 0; i < e->pos; i++) {
        if (e->buf.data[i] == '\\' && i + 1 < e->pos) {
            i++;
            continue;
        }
        if (quote || e->buf.data[i] == '\'' || e->buf.data[i] == '"') {
            quote = quote == e->buf.data[i] ? 0 : quote ? quote
                : e->buf.data[i];
            continue;
        }
        if (strchr(" \t;|&()<>", e->buf.data[i]))
            start = i + 1;
    }
    i = start;
    while (i > 0 && strchr(" \t", e->buf.data[i - 1]))
        i--;
    *command = i == 0 || strchr(";|&(", e->buf.data[i - 1]);
    return start;
}

static char *unescape(const char *s, size_t n)
{
    str_t out;

    str_init(&out);
    for (size_t i = 0; i < n; i++) {
        if (s[i] == '\'' || s[i] == '"')
            continue;
        if (s[i] == '\\' && i + 1 < n)
            i++;
        str_push(&out, s[i]);
    }
    return str_take(&out);
}

static void replace_word(editor_t *e, size_t start, const char *text,
    bool finished)
{
    str_t esc;

    str_init(&esc);
    for (size_t i = 0; text[i]; i++) {
        if (strchr(SPECIAL, text[i]) && !(text[i] == '$' && i == 0))
            str_push(&esc, '\\');
        str_push(&esc, text[i]);
    }
    if (finished && esc.len && esc.data[esc.len - 1] != '/')
        str_push(&esc, ' ');
    str_erase(&e->buf, start, e->pos - start);
    e->pos = start;
    edit_insert(e, esc.data, esc.len);
    str_free(&esc);
}

static size_t common_prefix(vec_t *v)
{
    size_t len = strlen(v->items[0]);

    for (size_t i = 1; i < v->len; i++)
        for (size_t j = 0; j < len; j++)
            if (v->items[i][j] != v->items[0][j]) {
                len = j;
                break;
            }
    return len;
}

static void list_choices(editor_t *e, vec_t *items)
{
    e->pos = e->buf.len;
    editor_refresh(e);
    write(STDOUT_FILENO, "\r\n", 2);
    term_restore();
    print_columns(items);
    term_raw();
    editor_redraw_all(e);
}

void complete(editor_t *e)
{
    bool command = false;
    size_t start = word_start(e, &command);
    char *word = unescape(e->buf.data + start, e->pos - start);
    vec_t items;
    size_t common = 0;

    vec_init(&items);
    complete_candidates(word, command, &items);
    if (items.len == 0)
        write(STDOUT_FILENO, "\a", 1);
    if (items.len == 1)
        replace_word(e, start, items.items[0], true);
    if (items.len > 1) {
        common = common_prefix(&items);
        if (common > strlen(word)) {
            items.items[0][common] = '\0';
            replace_word(e, start, items.items[0], false);
        } else
            list_choices(e, &items);
    }
    vec_free(&items);
    free(word);
}
