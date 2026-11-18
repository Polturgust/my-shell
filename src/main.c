/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** main.c - 42sh entry point
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

int shell_loop(void);

static void init_variables(void)
{
    char *cwd = getcwd(NULL, 0);

    set_status(0);
    if (cwd)
        set_variable(&sh()->vars, "cwd", cwd);
    if (env_get("HOME"))
        set_variable(&sh()->vars, "home", env_get("HOME"));
    set_variable(&sh()->vars, "shell", SHELL_NAME);
    free(cwd);
}

static void load_rc(void)
{
    const char *home = env_get("HOME");
    char *path = home ? path_join(home, ".myshrc") : NULL;

    bool abort = sh()->abort_on_error;

    sh()->abort_on_error = false;
    if (path && access(path, R_OK) == 0)
        run_file(path, true);
    else
        run_file(".myshrc", true);
    free(path);
    sh()->abort_on_error = abort;
    sh()->error_raised = false;
    set_status(0);
}

static int usage(void)
{
    dprintf(1, "usage: %s [-c command | file]\n", SHELL_NAME);
    return 0;
}

static void cleanup(void)
{
    if (sh()->interactive)
        history_save();
    history_clear();
    alias_free();
    env_free();
    free_variables(sh()->vars);
}

int main(int argc, char **argv, char **envp)
{
    int status = 0;

    env_init(envp);
    init_variables();
    if (argc > 1 && strcmp(argv[1], "-h") == 0)
        return usage();
    if (argc > 2 && strcmp(argv[1], "-c") == 0) {
        sh()->abort_on_error = true;
        status = run_line(argv[2]);
        cleanup();
        return status;
    }
    sh()->interactive = argc == 1 && isatty(STDIN_FILENO);
    sh()->abort_on_error = !sh()->interactive;
    jobs_init();
    if (sh()->interactive)
        history_load();
    load_rc();
    status = argc > 1 ? run_file(argv[1], false) : shell_loop();
    cleanup();
    return status;
}
