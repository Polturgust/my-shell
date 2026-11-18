/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** columns.c - print completion choices in columns, like ls
*/

#include <stdio.h>
#include <string.h>
#include "editor.h"

#define MAX_SHOWN 400

static const char *shown_name(const char *item)
{
    size_t len = strlen(item);
    const char *slash = NULL;

    if (len > 1 && item[len - 1] == '/') {
        for (size_t i = len - 1; i > 0; i--)
            if (item[i - 1] == '/')
                return item + i;
        return item;
    }
    slash = strrchr(item, '/');
    return slash ? slash + 1 : item;
}

void print_columns(vec_t *items)
{
    size_t n = items->len < MAX_SHOWN ? items->len : MAX_SHOWN;
    size_t width = 0;
    size_t cols = 0;
    size_t rows = 0;
    size_t idx = 0;

    for (size_t i = 0; i < n; i++)
        if (strlen(shown_name(items->items[i])) > width)
            width = strlen(shown_name(items->items[i]));
    width += 2;
    cols = (size_t)term_width() / width;
    cols = cols ? cols : 1;
    rows = (n + cols - 1) / cols;
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            idx = c * rows + r;
            if (idx < n)
                dprintf(1, "%-*s", (int)(c + 1 < cols ? width : 0),
                    shown_name(items->items[idx]));
        }
        dprintf(1, "\n");
    }
    if (items->len > n)
        dprintf(1, "... and %zu more\n", items->len - n);
}
