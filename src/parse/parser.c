/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** parser.c - lists, && / || chains and pipelines
**
** list     := and_or { (';' | '&') and_or }
** and_or   := pipeline { ('&&' | '||') pipeline }
** pipeline := command { '|' command }
*/

#include <stdlib.h>
#include "shell.h"

token_t *peek(parser_t *p)
{
    return &p->toks->items[p->pos];
}

node_t *parse_error(parser_t *p, const char *msg, node_t *partial)
{
    if (!p->error)
        print_error("%s\n", msg);
    p->error = true;
    node_free(partial);
    return NULL;
}

static node_t *binary(node_type_t type, node_t *left, node_t *right)
{
    node_t *node = node_new(type);

    node->left = left;
    node->right = right;
    return node;
}

static bool has_redir(node_t *cmd, bool input)
{
    for (redir_t *r = cmd->redirs; r; r = r->next) {
        if (input && (r->type == R_IN || r->type == R_HEREDOC))
            return true;
        if (!input && r->type != R_IN && r->type != R_HEREDOC
            && r->type != R_ERR && r->type != R_ERR_APPEND
            && r->type != R_ERR_TO_OUT)
            return true;
    }
    return false;
}

static node_t *finish_pipeline(parser_t *p, node_t *pipe)
{
    for (size_t i = 0; i < pipe->nkids; i++) {
        if (i > 0 && has_redir(pipe->kids[i], true))
            return parse_error(p, "Ambiguous input redirect.", pipe);
        if (i + 1 < pipe->nkids && has_redir(pipe->kids[i], false))
            return parse_error(p, "Ambiguous output redirect.", pipe);
    }
    if (pipe->nkids == 1) {
        node_t *only = pipe->kids[0];

        pipe->nkids = 0;
        node_free(pipe);
        return only;
    }
    return pipe;
}

static node_t *parse_pipeline(parser_t *p)
{
    node_t *pipe = node_new(N_PIPE);
    node_t *cmd = parse_command(p);

    while (!p->error && cmd) {
        pipe->kids = xrealloc(pipe->kids, sizeof(node_t *) * (pipe->nkids + 1));
        pipe->kids[pipe->nkids++] = cmd;
        if (peek(p)->type != T_PIPE)
            return finish_pipeline(p, pipe);
        p->pos++;
        cmd = parse_command(p);
        if (!cmd && !p->error)
            return parse_error(p, "Invalid null command.", pipe);
    }
    if (!p->error && peek(p)->type == T_PIPE)
        return parse_error(p, "Invalid null command.", pipe);
    node_free(pipe);
    return NULL;
}

static node_t *parse_and_or(parser_t *p)
{
    node_t *left = parse_pipeline(p);
    node_t *right = NULL;
    node_type_t type;

    if (!left && !p->error && (peek(p)->type == T_AND
        || peek(p)->type == T_OR))
        return parse_error(p, "Invalid null command.", NULL);
    while (left && (peek(p)->type == T_AND || peek(p)->type == T_OR)) {
        type = peek(p)->type == T_AND ? N_AND : N_OR;
        p->pos++;
        right = parse_pipeline(p);
        if (!right)
            return parse_error(p, "Invalid null command.", left);
        left = binary(type, left, right);
    }
    return left;
}

node_t *parse_list(parser_t *p)
{
    node_t *list = NULL;
    node_t *item = NULL;

    while (!p->error && peek(p)->type != T_END
        && peek(p)->type != T_RPAREN) {
        if (peek(p)->type == T_SEMI || peek(p)->type == T_BG) {
            p->pos++;
            continue;
        }
        item = parse_and_or(p);
        if (!item)
            return p->error ? parse_error(p, "", list) : list;
        if (peek(p)->type == T_BG)
            item = binary(N_BG, item, NULL);
        list = list ? binary(N_SEQ, list, item) : item;
        if (peek(p)->type == T_LPAREN)
            return parse_error(p, "Badly placed ()'s.", list);
    }
    return list;
}
