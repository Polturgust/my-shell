/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** heredoc.c - read << bodies after the command line, feed them on stdin
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

static char *unquote(const char *word)
{
    str_t out;

    str_init(&out);
    for (size_t i = 0; word[i]; i++) {
        if (word[i] == '\'' || word[i] == '"')
            continue;
        if (word[i] == '\\' && word[i + 1])
            i++;
        str_push(&out, word[i]);
    }
    return str_take(&out);
}

static int read_body(redir_t *r)
{
    char *delim = unquote(r->target);
    char *line = NULL;
    bool eof = false;
    str_t body;

    str_init(&body);
    while (true) {
        line = read_input_line("? ", &eof);
        if (eof || !line || strcmp(line, delim) == 0)
            break;
        str_append(&body, line);
        str_push(&body, '\n');
        free(line);
    }
    free(line);
    free(delim);
    r->body = str_take(&body);
    return 0;
}

int read_heredocs(node_t *node)
{
    if (!node)
        return 0;
    for (redir_t *r = node->redirs; r; r = r->next)
        if (r->type == R_HEREDOC && read_body(r) < 0)
            return -1;
    for (size_t i = 0; i < node->nkids; i++)
        if (read_heredocs(node->kids[i]) < 0)
            return -1;
    if (read_heredocs(node->left) < 0)
        return -1;
    return read_heredocs(node->right);
}

static void write_all(int fd, const char *data, size_t len)
{
    ssize_t n = 0;

    while (len > 0 && (n = write(fd, data, len)) > 0) {
        data += n;
        len -= n;
    }
}

int heredoc_fd(redir_t *r)
{
    char *body = r->quoted ? xstrdup(r->body) : expand_heredoc(r->body);
    int fds[2];
    size_t len = 0;

    if (!body || pipe(fds) < 0) {
        free(body);
        return -1;
    }
    len = strlen(body);
    if (len < 60000) {
        write_all(fds[1], body, len);
    } else if (fork() == 0) {
        close(fds[0]);
        write_all(fds[1], body, len);
        exit(0);
    }
    close(fds[1]);
    free(body);
    return fds[0];
}
