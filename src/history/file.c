/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** file.c - keep the history in ~/.42sh_history between sessions
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

static char *history_path(void)
{
    const char *home = env_get("HOME");

    return home ? path_join(home, HISTORY_FILE) : NULL;
}

void history_load(void)
{
    char *path = history_path();
    FILE *file = path ? fopen(path, "r") : NULL;
    char *line = NULL;
    size_t size = 0;
    ssize_t len = 0;

    free(path);
    if (!file)
        return;
    while ((len = getline(&line, &size, file)) > 0) {
        if (line[len - 1] == '\n')
            line[len - 1] = '\0';
        history_add(line);
    }
    free(line);
    fclose(file);
}

void history_save(void)
{
    char *path = history_path();
    FILE *file = path ? fopen(path, "w") : NULL;

    free(path);
    if (!file)
        return;
    for (size_t i = 0; i < history_len(); i++)
        fprintf(file, "%s\n", history_at(i)->cmd);
    fclose(file);
}
