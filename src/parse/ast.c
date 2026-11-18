/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** ast.c - syntax tree nodes
*/

#include <stdlib.h>
#include <string.h>
#include "shell.h"

node_t *node_new(node_type_t type)
{
    node_t *node = xmalloc(sizeof(node_t));

    memset(node, 0, sizeof(node_t));
    node->type = type;
    if (type == N_CMD)
        vec_init(&node->argv);
    return node;
}

static void redirs_free(redir_t *r)
{
    redir_t *next = NULL;

    while (r) {
        next = r->next;
        free(r->target);
        free(r->body);
        free(r);
        r = next;
    }
}

void node_free(node_t *node)
{
    if (!node)
        return;
    vec_free(&node->argv);
    redirs_free(node->redirs);
    node_free(node->left);
    node_free(node->right);
    for (size_t i = 0; i < node->nkids; i++)
        node_free(node->kids[i]);
    free(node->kids);
    free(node->text);
    free(node);
}

node_t *parse_line(const char *line, tokens_t *toks, bool *error)
{
    parser_t p = {toks, 0, false};
    node_t *root = NULL;

    *error = lex_line(line, toks) < 0;
    if (*error)
        return NULL;
    root = parse_list(&p);
    if (!p.error && peek(&p)->type == T_RPAREN)
        root = parse_error(&p, "Too many )'s.", root);
    *error = p.error;
    return p.error ? NULL : root;
}
