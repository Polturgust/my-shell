/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** history.c - the list of commands typed so far
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

typedef struct history_s {
    hist_entry_t *items;
    size_t len;
    size_t cap;
    int next_id;
} history_t;

static history_t *hist(void)
{
    static history_t h = {NULL, 0, 0, 1};

    return &h;
}

static void drop_oldest(history_t *h)
{
    free(h->items[0].cmd);
    memmove(&h->items[0], &h->items[1], sizeof(hist_entry_t) * (h->len - 1));
    h->len--;
}

void history_add(const char *cmd)
{
    history_t *h = hist();

    if (!cmd[strspn(cmd, " \t")])
        return;
    if (h->len && strcmp(h->items[h->len - 1].cmd, cmd) == 0)
        return;
    if (h->len >= HISTORY_MAX)
        drop_oldest(h);
    if (h->len + 1 > h->cap) {
        h->cap = h->cap ? h->cap * 2 : 64;
        h->items = xrealloc(h->items, sizeof(hist_entry_t) * h->cap);
    }
    h->items[h->len] = (hist_entry_t){h->next_id++, time(NULL), xstrdup(cmd)};
    h->len++;
}

size_t history_len(void)
{
    return hist()->len;
}

const hist_entry_t *history_at(size_t index)
{
    if (index >= hist()->len)
        return NULL;
    return &hist()->items[index];
}

void history_clear(void)
{
    while (hist()->len)
        drop_oldest(hist());
}
