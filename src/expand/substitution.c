/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** substitution.c - run a command and capture what it prints
*/

#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

static void run_capture_child(const char *cmd, int pipefd[2])
{
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    reset_child_signals();
    sh()->in_child = true;
    sh()->interactive = false;
    sh()->job_control = false;
    sh()->abort_on_error = true;
    sh()->error_raised = false;
    exit(run_line(cmd));
}

char *command_substitution(const char *cmd)
{
    int pipefd[2];
    pid_t pid = 0;
    str_t out;
    char buf[4096];
    ssize_t n = 0;

    str_init(&out);
    if (pipe(pipefd) < 0)
        return str_take(&out);
    pid = fork();
    if (pid == 0)
        run_capture_child(cmd, pipefd);
    close(pipefd[1]);
    while (pid > 0 && (n = read(pipefd[0], buf, sizeof(buf))) > 0)
        str_appendn(&out, buf, n);
    close(pipefd[0]);
    if (pid > 0)
        waitpid(pid, NULL, 0);
    while (out.len && out.data[out.len - 1] == '\n')
        out.data[--out.len] = '\0';
    return str_take(&out);
}

char *expand_tilde(const char *word, size_t *consumed)
{
    size_t end = 1;
    char *user = NULL;
    struct passwd *pw = NULL;

    while (word[end] && word[end] != '/')
        end++;
    *consumed = end;
    if (end == 1)
        return env_get("HOME") ? xstrdup(env_get("HOME")) : NULL;
    user = xstrndup(word + 1, end - 1);
    pw = getpwnam(user);
    free(user);
    return pw ? xstrdup(pw->pw_dir) : NULL;
}

char *expand_heredoc(const char *body)
{
    str_t out;
    size_t i = 0;

    str_init(&out);
    while (body[i]) {
        if (body[i] == '\\' && body[i + 1] && strchr("$`\\", body[i + 1])) {
            str_push(&out, body[i + 1]);
            i += 2;
            continue;
        }
        if (body[i] != '$') {
            str_push(&out, body[i++]);
            continue;
        }
        if (expand_dollar(body, &i, &out) < 0) {
            str_free(&out);
            return NULL;
        }
    }
    return str_take(&out);
}
