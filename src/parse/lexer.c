/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** lexer.c - split a command line into words and operators
*/

#include <stdlib.h>
#include <string.h>
#include "shell.h"

typedef struct op_s {
    const char *text;
    tok_type_t type;
    redir_type_t redir;
} op_t;

static const op_t OPERATORS[] = {
    {"2>&1", T_REDIR, R_ERR_TO_OUT}, {"2>>", T_REDIR, R_ERR_APPEND},
    {"2>", T_REDIR, R_ERR}, {">>&", T_REDIR, R_APPEND_ERR},
    {">&", T_REDIR, R_OUT_ERR}, {">>", T_REDIR, R_APPEND},
    {">", T_REDIR, R_OUT}, {"<<", T_REDIR, R_HEREDOC},
    {"<", T_REDIR, R_IN}, {"||", T_OR, 0}, {"|", T_PIPE, 0},
    {"&&", T_AND, 0}, {"&", T_BG, 0}, {";", T_SEMI, 0},
    {"\n", T_SEMI, 0}, {"(", T_LPAREN, 0}, {")", T_RPAREN, 0},
    {NULL, 0, 0}
};

void tokens_push(tokens_t *t, token_t tok)
{
    if (t->len + 1 >= t->cap) {
        t->cap = t->cap ? t->cap * 2 : 16;
        t->items = xrealloc(t->items, sizeof(token_t) * t->cap);
    }
    t->items[t->len] = tok;
    t->len++;
}

void tokens_free(tokens_t *t)
{
    for (size_t i = 0; i < t->len; i++)
        free(t->items[i].text);
    free(t->items);
    t->items = NULL;
    t->len = 0;
    t->cap = 0;
}

static const op_t *match_operator(const char *s, bool word_start)
{
    for (size_t i = 0; OPERATORS[i].text; i++) {
        if (OPERATORS[i].text[0] == '2' && !word_start)
            continue;
        if (strncmp(s, OPERATORS[i].text, strlen(OPERATORS[i].text)) == 0)
            return &OPERATORS[i];
    }
    return NULL;
}

static int lex_word(const char *line, size_t *i, tokens_t *out)
{
    size_t start = *i;
    token_t tok = {T_WORD, 0, NULL, false, 0};

    if (scan_word_end(line, i, &tok.quoted) < 0)
        return -1;
    tok.text = xstrndup(line + start, *i - start);
    tokens_push(out, tok);
    return 0;
}

int lex_line(const char *line, tokens_t *out)
{
    size_t i = 0;
    const op_t *op = NULL;

    while (line[i]) {
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\r') {
            i++;
            continue;
        }
        if (line[i] == '#')
            break;
        op = match_operator(line + i, true);
        if (op) {
            tokens_push(out, (token_t){op->type, op->redir, NULL, false, 0});
            i += strlen(op->text);
            continue;
        }
        if (lex_word(line, &i, out) < 0)
            return -1;
    }
    tokens_push(out, (token_t){T_END, 0, NULL, false, 0});
    return 0;
}
