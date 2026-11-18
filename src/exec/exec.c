/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** exec.c - walk the syntax tree
*/

#include <stdlib.h>
#include "shell.h"

static int exec_background(node_t *node)
{
    char *text = node_to_text(node);
    int ret = 0;

    if (node->type == N_PIPE)
        ret = exec_pipeline(node->kids, node->nkids, text, true);
    else
        ret = exec_pipeline(&node, 1, text, true);
    free(text);
    return ret;
}

static int exec_chain(node_t *node)
{
    int status = exec_node(node->left);

    if (sh()->do_exit || should_abort())
        return status;
    if ((node->type == N_AND && status == 0)
        || (node->type == N_OR && status != 0)
        || node->type == N_SEQ)
        if (node->right)
            return exec_node(node->right);
    return status;
}

int exec_node(node_t *node)
{
    char *text = NULL;
    int status = 0;

    if (!node || should_abort())
        return sh()->status;
    if (node->type == N_SEQ || node->type == N_AND || node->type == N_OR)
        status = exec_chain(node);
    else if (node->type == N_BG)
        status = exec_background(node->left);
    else {
        text = node_to_text(node);
        if (node->type == N_PIPE)
            status = exec_pipeline(node->kids, node->nkids, text, false);
        else
            status = exec_pipeline(&node, 1, text, false);
        free(text);
    }
    set_status(status);
    return status;
}
