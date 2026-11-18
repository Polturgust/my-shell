/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** run.c - run one line of input, or a whole file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

shell_t *sh(void)
{
    static shell_t shell;
    static bool init = false;

    if (!init) {
        memset(&shell, 0, sizeof(shell));
        shell.input = stdin;
        init = true;
    }
    return &shell;
}

int run_line(const char *line)
{
    tokens_t toks = {NULL, 0, 0};
    bool error = false;
    node_t *root = parse_line(line, &toks, &error);

    tokens_free(&toks);
    if (error) {
        set_status(1);
        should_abort();
        return 1;
    }
    if (!root)
        return sh()->status;
    if (read_heredocs(root) == 0)
        exec_node(root);
    node_free(root);
    if (should_abort())
        set_status(1);
    return sh()->status;
}

int run_file(const char *path, bool quiet_missing)
{
    FILE *file = fopen(path, "r");
    FILE *previous = sh()->input;
    bool interactive = sh()->interactive;
    char *line = NULL;
    bool eof = false;

    if (!file)
        return quiet_missing ? 0 : print_error("%s: No such file or "
            "directory.\n", path);
    sh()->input = file;
    sh()->interactive = false;
    while (!sh()->do_exit) {
        line = read_input_line(NULL, &eof);
        if (eof)
            break;
        run_line(line);
        free(line);
    }
    free(line);
    fclose(file);
    sh()->input = previous;
    sh()->interactive = interactive;
    return sh()->status;
}
