/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** alias_expand.c - replace an alias in command position by its value
*/

#include <stdlib.h>
#include <string.h>
#include "shell.h"

static void splice(tokens_t *toks, size_t pos, tokens_t *sub)
{
    size_t tail = toks->len - pos - 1;

    while (toks->len + sub->len >= toks->cap) {
        toks->cap *= 2;
        toks->items = xrealloc(toks->items, sizeof(token_t) * toks->cap);
    }
    free(toks->items[pos].text);
    memmove(&toks->items[pos + sub->len], &toks->items[pos + 1],
        sizeof(token_t) * tail);
    memcpy(&toks->items[pos], sub->items, sizeof(token_t) * sub->len);
    toks->len = toks->len - 1 + sub->len;
}

static int expand_once(parser_t *p, const char *value)
{
    token_t *tok = peek(p);
    tokens_t sub = {NULL, 0, 0};

    if (lex_line(value, &sub) < 0) {
        tokens_free(&sub);
        return -1;
    }
    sub.len--;
    for (size_t i = 0; i < sub.len; i++)
        sub.items[i].alias_depth = tok->alias_depth + 1;
    if (sub.len && sub.items[0].type == T_WORD
        && strcmp(sub.items[0].text, tok->text) == 0)
        sub.items[0].alias_depth = -1;
    splice(p->toks, p->pos, &sub);
    free(sub.items);
    return 0;
}

int expand_alias_at(parser_t *p)
{
    const char *value = NULL;
    token_t *tok = NULL;

    while (true) {
        tok = peek(p);
        if (tok->type != T_WORD || tok->quoted || tok->alias_depth < 0)
            return 0;
        value = alias_get(tok->text);
        if (!value)
            return 0;
        if (tok->alias_depth >= ALIAS_DEPTH_MAX) {
            print_error("Alias loop.\n");
            return -1;
        }
        if (expand_once(p, value) < 0)
            return -1;
    }
}
