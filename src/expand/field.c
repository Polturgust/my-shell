/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** field.c - build fields, keep a parallel glob pattern where quoted
**           characters are escaped
*/

#include <glob.h>
#include <stdlib.h>
#include <string.h>
#include "field.h"

void field_literal(field_t *f, char c)
{
    str_push(&f->cur, c);
    if (strchr("*?[]\\", c))
        str_push(&f->pat, '\\');
    str_push(&f->pat, c);
    f->started = true;
}

void field_literals(field_t *f, const char *text)
{
    for (size_t i = 0; text[i]; i++)
        field_literal(f, text[i]);
    f->started = true;
}

void field_split_append(field_t *f, const char *text)
{
    for (size_t i = 0; text[i]; i++) {
        if (strchr(" \t\n", text[i])) {
            field_end(f);
            continue;
        }
        field_literal(f, text[i]);
    }
}

static int push_matches(field_t *f)
{
    glob_t g;
    int ret = glob(f->pat.data, 0, NULL, &g);

    if (ret != 0) {
        if (ret != GLOB_NOMATCH)
            globfree(&g);
        return -2;
    }
    for (size_t i = 0; i < g.gl_pathc; i++)
        vec_push(f->out, xstrdup(g.gl_pathv[i]));
    globfree(&g);
    return 0;
}

static bool is_pattern(const char *pat)
{
    for (size_t i = 0; pat[i]; i++) {
        if (pat[i] == '\\' && pat[i + 1]) {
            i++;
            continue;
        }
        if (pat[i] == '*' || pat[i] == '?')
            return true;
        if (pat[i] == '[' && pat[i + 1] && pat[i + 1] != ']'
            && strchr(pat + i + 2, ']'))
            return true;
    }
    return false;
}

int field_end(field_t *f)
{
    int ret = 0;

    if (f->has_glob && is_pattern(f->pat.data))
        ret = push_matches(f);
    else if (f->started || f->cur.len)
        vec_push(f->out, xstrdup(f->cur.data));
    str_set(&f->cur, "");
    str_set(&f->pat, "");
    f->has_glob = false;
    f->started = false;
    return ret;
}
