/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** builtin.c - history [-chr] [n]
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

static void print_entry(const hist_entry_t *e, bool numbers)
{
    struct tm *t = localtime(&e->time);

    if (numbers)
        dprintf(1, "%6d\t%2d:%02d\t%s\n", e->id, t->tm_hour, t->tm_min,
            e->cmd);
    else
        dprintf(1, "%s\n", e->cmd);
}

static int parse_flags(char **argv, bool flags[3], long *count)
{
    for (size_t i = 1; argv[i]; i++) {
        if (argv[i][0] != '-') {
            if (strspn(argv[i], "0123456789") != strlen(argv[i]))
                return print_error("history: Badly formed number.\n");
            *count = atol(argv[i]);
            continue;
        }
        for (size_t j = 1; argv[i][j]; j++) {
            if (!strchr("chr", argv[i][j]))
                return print_error("Usage: history [-chr] [# number of "
                    "events].\n");
            flags[strchr("chr", argv[i][j]) - "chr"] = true;
        }
    }
    return 0;
}

int builtin_history(char **argv)
{
    bool flags[3] = {false, false, false};
    long count = -1;
    size_t len = history_len();
    size_t start = 0;

    if (parse_flags(argv, flags, &count))
        return 1;
    if (flags[0]) {
        history_clear();
        return 0;
    }
    if (count >= 0 && (size_t)count < len)
        start = len - count;
    for (size_t i = start; i < len; i++)
        print_entry(history_at(flags[2] ? len - 1 - (i - start) : i),
            !flags[1]);
    return 0;
}
