/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** misc.c - character classes and paths
*/

#include <string.h>
#include "shell.h"

bool is_name_char(char c, bool first)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
        return true;
    return !first && c >= '0' && c <= '9';
}

char *path_join(const char *dir, const char *name)
{
    size_t dlen = strlen(dir);
    size_t nlen = strlen(name);
    bool slash = dlen > 0 && dir[dlen - 1] != '/';
    char *out = xmalloc(dlen + nlen + 2);

    memcpy(out, dir, dlen);
    if (slash)
        out[dlen] = '/';
    memcpy(out + dlen + slash, name, nlen + 1);
    return out;
}
