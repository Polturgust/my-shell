/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** input.c - read a line from the terminal editor or from a file / pipe
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

static char *read_plain(FILE *input, bool *eof)
{
    char *line = NULL;
    size_t size = 0;
    ssize_t len = getline(&line, &size, input);

    if (len < 0) {
        free(line);
        *eof = true;
        return NULL;
    }
    if (len && line[len - 1] == '\n')
        line[len - 1] = '\0';
    return line;
}

char *read_input_line(const char *prompt, bool *eof)
{
    *eof = false;
    if (sh()->interactive && sh()->input == stdin)
        return editor_read(prompt ? prompt : "", eof);
    return read_plain(sh()->input, eof);
}

void print_prompt_header(void)
{
    char *cwd = getcwd(NULL, 0);
    const char *home = env_get("HOME");
    size_t hlen = home ? strlen(home) : 0;

    if (cwd && hlen > 1 && strncmp(cwd, home, hlen) == 0
        && (cwd[hlen] == '/' || !cwd[hlen]))
        dprintf(STDOUT_FILENO, "\033[35m~%s\033[0m\n", cwd + hlen);
    else
        dprintf(STDOUT_FILENO, "\033[35m%s\033[0m\n", cwd ? cwd : "?");
    free(cwd);
}
