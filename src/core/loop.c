/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** loop.c - the read / run loop
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.h"

#define PROMPT "\033[36m\xe2\x9e\x9c\033[0m "

static char *prepare_line(char *line)
{
    char *expanded = NULL;
    int ret = 0;

    if (!sh()->interactive)
        return line;
    ret = history_expand(line, &expanded);
    if (ret < 0) {
        free(line);
        set_status(1);
        return NULL;
    }
    free(line);
    if (ret > 0)
        dprintf(STDOUT_FILENO, "%s\n", expanded);
    history_add(expanded);
    return expanded;
}

static bool handle_eof(void)
{
    if (sh()->interactive && find_variable(sh()->vars, "ignoreeof")) {
        dprintf(STDOUT_FILENO, "Use \"exit\" to leave %s.\n", SHELL_NAME);
        return false;
    }
    if (sh()->interactive && jobs_any_stopped() && !sh()->warned_jobs) {
        sh()->warned_jobs = true;
        print_error("There are suspended jobs.\n");
        return false;
    }
    if (sh()->interactive)
        dprintf(STDOUT_FILENO, "exit\n");
    return true;
}

int shell_loop(void)
{
    char *line = NULL;
    bool eof = false;

    while (!sh()->do_exit) {
        jobs_reap(sh()->interactive);
        if (sh()->interactive) {
            run_alias_hook("precmd");
            print_prompt_header();
        }
        line = read_input_line(PROMPT, &eof);
        if (eof && handle_eof())
            break;
        if (eof)
            continue;
        sh()->error_raised = false;
        line = prepare_line(line);
        if (line)
            run_line(line);
        free(line);
    }
    return sh()->status;
}
