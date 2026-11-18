/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** text.c - turn a syntax tree back into a command line (for job messages)
*/

#include <stdlib.h>
#include "shell.h"

static const char *REDIR_TEXT[] = {
    "<", ">", ">>", "<<", ">&", ">>&", "2>", "2>>", "2>&1"
};

static void text_cmd(const node_t *node, str_t *out)
{
    for (size_t i = 0; i < node->argv.len; i++) {
        if (i)
            str_push(out, ' ');
        str_append(out, node->argv.items[i]);
    }
    for (redir_t *r = node->redirs; r; r = r->next) {
        str_push(out, ' ');
        str_append(out, REDIR_TEXT[r->type]);
        if (r->target) {
            str_push(out, ' ');
            str_append(out, r->target);
        }
    }
}

static void text_node(const node_t *node, str_t *out);

static void text_binary(const node_t *node, str_t *out, const char *op)
{
    text_node(node->left, out);
    if (!node->right) {
        str_append(out, op);
        return;
    }
    str_append(out, op);
    text_node(node->right, out);
}

static void text_node(const node_t *node, str_t *out)
{
    if (!node)
        return;
    if (node->type == N_CMD)
        return text_cmd(node, out);
    if (node->type == N_SUBSHELL) {
        str_push(out, '(');
        text_node(node->left, out);
        str_push(out, ')');
        return;
    }
    for (size_t i = 0; node->type == N_PIPE && i < node->nkids; i++) {
        if (i)
            str_append(out, " | ");
        text_node(node->kids[i], out);
    }
    if (node->type == N_AND || node->type == N_OR || node->type == N_SEQ)
        text_binary(node, out, node->type == N_AND ? " && "
            : node->type == N_OR ? " || " : "; ");
    if (node->type == N_BG)
        text_binary(node, out, " &");
}

char *node_to_text(const node_t *node)
{
    str_t out;

    str_init(&out);
    text_node(node, &out);
    return str_take(&out);
}
