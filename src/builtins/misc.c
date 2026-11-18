/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** misc.c - echo, exit, source, repeat
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int builtin_echo(char **argv)
{
    bool newline = true;
    size_t i = 1;

    if (argv[1] && strcmp(argv[1], "-n") == 0) {
        newline = false;
        i++;
    }
    for (size_t first = i; argv[i]; i++)
        dprintf(1, "%s%s", i == first ? "" : " ", argv[i]);
    if (newline)
        dprintf(1, "\n");
    return 0;
}

static bool is_number(const char *s)
{
    size_t i = (s[0] == '-' || s[0] == '+');

    if (!s[i])
        return false;
    for (; s[i]; i++)
        if (s[i] < '0' || s[i] > '9')
            return false;
    return true;
}

int builtin_exit(char **argv)
{
    if (argv[1] && (argv[2] || !is_number(argv[1])))
        return print_error("exit: Expression Syntax.\n");
    if (sh()->interactive && !sh()->in_child && jobs_any_stopped()
        && !sh()->warned_jobs) {
        sh()->warned_jobs = true;
        return print_error("There are suspended jobs.\n");
    }
    sh()->do_exit = true;
    return argv[1] ? (atoi(argv[1]) & 0xff) : 0;
}

int builtin_source(char **argv)
{
    if (!argv[1])
        return print_error("source: Too few arguments.\n");
    return run_file(argv[1], false);
}

int builtin_repeat(char **argv)
{
    str_t line;
    int count = 0;
    int status = 0;

    if (!argv[1] || !argv[2])
        return print_error("repeat: Too few arguments.\n");
    if (!is_number(argv[1]))
        return print_error("repeat: Badly formed number.\n");
    count = atoi(argv[1]);
    str_init(&line);
    for (size_t i = 2; argv[i]; i++) {
        str_append(&line, i > 2 ? " " : "");
        str_append(&line, argv[i]);
    }
    for (int i = 0; i < count && !sh()->do_exit; i++)
        status = run_line(line.data);
    str_free(&line);
    return status;
}
