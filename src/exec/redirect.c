/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** redirect.c - <, >, >>, <<, >&, >>&, 2>, 2>>, 2>&1
*/

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

int heredoc_fd(redir_t *r);

static int open_target(redir_t *r)
{
    char *path = expand_single(r->target);
    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    int fd = -1;

    if (!path)
        return -1;
    if (r->type == R_IN)
        flags = O_RDONLY;
    if (r->type == R_APPEND || r->type == R_APPEND_ERR
        || r->type == R_ERR_APPEND)
        flags = O_WRONLY | O_CREAT | O_APPEND;
    fd = open(path, flags, 0644);
    if (fd < 0)
        print_error("%s: %s.\n", path, strerror(errno));
    free(path);
    return fd;
}

static int apply_one(redir_t *r)
{
    int fd = -1;
    int target = STDOUT_FILENO;

    if (r->type == R_ERR_TO_OUT)
        return dup2(STDOUT_FILENO, STDERR_FILENO) < 0 ? -1 : 0;
    fd = r->type == R_HEREDOC ? heredoc_fd(r) : open_target(r);
    if (fd < 0)
        return -1;
    if (r->type == R_IN || r->type == R_HEREDOC)
        target = STDIN_FILENO;
    if (r->type == R_ERR || r->type == R_ERR_APPEND)
        target = STDERR_FILENO;
    dup2(fd, target);
    close(fd);
    if (r->type == R_OUT_ERR || r->type == R_APPEND_ERR)
        dup2(STDOUT_FILENO, STDERR_FILENO);
    return 0;
}

int apply_redirs(redir_t *redirs)
{
    for (redir_t *r = redirs; r; r = r->next)
        if (apply_one(r) < 0)
            return -1;
    return 0;
}

int save_fds(int saved[3])
{
    for (int i = 0; i < 3; i++) {
        saved[i] = fcntl(i, F_DUPFD_CLOEXEC, 10);
        if (saved[i] < 0 && errno != EBADF)
            return -1;
    }
    return 0;
}

void restore_fds(int saved[3])
{
    for (int i = 0; i < 3; i++) {
        if (saved[i] < 0)
            continue;
        dup2(saved[i], i);
        close(saved[i]);
    }
}
