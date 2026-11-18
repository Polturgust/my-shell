/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** simple.c - one command: builtin in the shell itself, program in a child
*/

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

void exec_failed(const char *name)
{
    if (errno == ENOEXEC)
        print_error("%s: Exec format error. Binary file not executable.\n",
            name);
    else if (errno == ENOENT)
        print_notice("%s: Command not found.\n", name);
    else
        print_error("%s: %s.\n", name, strerror(errno));
    exit(errno == ENOENT ? 1 : 126);
}

static int run_builtin(builtin_fn_t fn, node_t *cmd, char **argv)
{
    int saved[3];
    int status = 1;

    if (save_fds(saved) < 0)
        return 1;
    if (apply_redirs(cmd->redirs) == 0)
        status = fn(argv);
    restore_fds(saved);
    return status;
}

int exec_simple(node_t *cmd)
{
    vec_t argv;
    builtin_fn_t fn = NULL;
    char *path = NULL;
    char *text = NULL;
    int status = 1;

    vec_init(&argv);
    status = expand_words(&cmd->argv, &argv) < 0 ? 1 : 0;
    if (status != 0 || argv.len == 0) {
        vec_free(&argv);
        return status;
    }
    status = 1;
    fn = builtin_find(argv.items[0]);
    if (fn)
        status = run_builtin(fn, cmd, argv.items);
    else if (!(path = find_command(argv.items[0])))
        print_notice("%s: Command not found.\n", argv.items[0]);
    else {
        text = node_to_text(cmd);
        status = launch_single(cmd, argv.items, path, text);
        free(text);
    }
    free(path);
    vec_free(&argv);
    return status;
}

void exec_in_child(node_t *cmd)
{
    vec_t argv;
    builtin_fn_t fn = NULL;
    char *path = NULL;

    if (apply_redirs(cmd->redirs) < 0)
        exit(1);
    if (cmd->type != N_CMD)
        exit(exec_node(cmd->type == N_SUBSHELL ? cmd->left : cmd));
    vec_init(&argv);
    if (expand_words(&cmd->argv, &argv) < 0 || argv.len == 0)
        exit(1);
    fn = builtin_find(argv.items[0]);
    if (fn)
        exit(fn(argv.items));
    path = find_command(argv.items[0]);
    if (!path) {
        print_notice("%s: Command not found.\n", argv.items[0]);
        exit(1);
    }
    execve(path, argv.items, env_array());
    exec_failed(argv.items[0]);
}
