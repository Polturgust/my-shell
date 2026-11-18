/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** command.c - simple commands, sub-shells and redirections
*/

#include <stdlib.h>
#include "shell.h"

static bool is_output(redir_type_t t)
{
    return t == R_OUT || t == R_APPEND || t == R_OUT_ERR
        || t == R_APPEND_ERR;
}

static int check_ambiguous(parser_t *p, node_t *cmd, redir_type_t type)
{
    bool input = type == R_IN || type == R_HEREDOC;

    for (redir_t *r = cmd->redirs; r; r = r->next) {
        if (input && (r->type == R_IN || r->type == R_HEREDOC)) {
            parse_error(p, "Ambiguous input redirect.", NULL);
            return -1;
        }
        if (is_output(type) && is_output(r->type)) {
            parse_error(p, "Ambiguous output redirect.", NULL);
            return -1;
        }
    }
    return 0;
}

static int parse_redir(parser_t *p, node_t *cmd)
{
    redir_t *r = xmalloc(sizeof(redir_t));
    redir_t **tail = &cmd->redirs;

    *r = (redir_t){peek(p)->redir, NULL, NULL, false, NULL};
    p->pos++;
    if (r->type != R_ERR_TO_OUT) {
        if (peek(p)->type != T_WORD) {
            free(r);
            parse_error(p, "Missing name for redirect.", NULL);
            return -1;
        }
        r->target = xstrdup(peek(p)->text);
        r->quoted = peek(p)->quoted;
        p->pos++;
    }
    if (check_ambiguous(p, cmd, r->type) < 0) {
        free(r->target);
        free(r);
        return -1;
    }
    while (*tail)
        tail = &(*tail)->next;
    *tail = r;
    return 0;
}

static node_t *parse_subshell(parser_t *p)
{
    node_t *node = node_new(N_SUBSHELL);

    p->pos++;
    node->left = parse_list(p);
    if (p->error)
        return parse_error(p, "", node);
    if (peek(p)->type != T_RPAREN)
        return parse_error(p, "Too many ('s.", node);
    p->pos++;
    if (!node->left)
        return parse_error(p, "Invalid null command.", node);
    while (peek(p)->type == T_REDIR)
        if (parse_redir(p, node) < 0)
            return parse_error(p, "", node);
    if (peek(p)->type == T_WORD || peek(p)->type == T_LPAREN)
        return parse_error(p, "Badly placed ()'s.", node);
    return node;
}

static node_t *parse_simple(parser_t *p)
{
    node_t *cmd = node_new(N_CMD);

    while (peek(p)->type == T_WORD || peek(p)->type == T_REDIR) {
        if (peek(p)->type == T_REDIR) {
            if (parse_redir(p, cmd) < 0)
                return parse_error(p, "", cmd);
            continue;
        }
        vec_push(&cmd->argv, xstrdup(peek(p)->text));
        p->pos++;
    }
    if (peek(p)->type == T_LPAREN)
        return parse_error(p, "Badly placed ()'s.", cmd);
    if (cmd->argv.len == 0)
        return parse_error(p, "Invalid null command.", cmd);
    return cmd;
}

node_t *parse_command(parser_t *p)
{
    if (expand_alias_at(p) < 0) {
        p->error = true;
        return NULL;
    }
    if (peek(p)->type == T_LPAREN)
        return parse_subshell(p);
    if (peek(p)->type == T_WORD || peek(p)->type == T_REDIR)
        return parse_simple(p);
    return NULL;
}
