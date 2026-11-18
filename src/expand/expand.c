/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** expand.c - quotes, variables, command substitution, ~ and globbing
*/

#include <stdlib.h>
#include <string.h>
#include "field.h"

static int expand_backquote(const char *s, size_t *i, str_t *out)
{
    size_t start = *i + 1;
    size_t end = start;
    str_t cmd;
    char *result = NULL;

    str_init(&cmd);
    for (; s[end] && s[end] != '`'; end++) {
        if (s[end] == '\\' && (s[end + 1] == '`' || s[end + 1] == '\\'))
            end++;
        str_push(&cmd, s[end]);
    }
    *i = s[end] ? end + 1 : end;
    result = command_substitution(cmd.data);
    str_free(&cmd);
    str_append(out, result);
    free(result);
    return 0;
}

static int dq_char(const char *s, size_t *i, field_t *f)
{
    str_t tmp;
    int ret = 0;

    if (s[*i] == '\\' && s[*i + 1] && strchr("$\"\\`", s[*i + 1])) {
        field_literal(f, s[*i + 1]);
        *i += 2;
        return 0;
    }
    if (s[*i] != '$' && s[*i] != '`') {
        field_literal(f, s[(*i)++]);
        return 0;
    }
    str_init(&tmp);
    ret = s[*i] == '$' ? expand_dollar(s, i, &tmp)
        : expand_backquote(s, i, &tmp);
    field_literals(f, tmp.data);
    str_free(&tmp);
    return ret;
}

static int expand_dquote(const char *s, size_t *i, field_t *f)
{
    f->started = true;
    for ((*i)++; s[*i] && s[*i] != '"';)
        if (dq_char(s, i, f) < 0)
            return -1;
    if (s[*i])
        (*i)++;
    return 0;
}

static int expand_unquoted(const char *s, size_t *i, field_t *f)
{
    str_t tmp;
    int ret = 0;

    str_init(&tmp);
    ret = s[*i] == '$' ? expand_dollar(s, i, &tmp)
        : expand_backquote(s, i, &tmp);
    field_split_append(f, tmp.data);
    str_free(&tmp);
    return ret;
}

int expand_char(const char *s, size_t *i, field_t *f)
{
    char c = s[*i];

    if (c == '\\') {
        if (s[*i + 1])
            field_literal(f, s[++(*i)]);
        (*i)++;
        return 0;
    }
    if (c == '\'') {
        f->started = true;
        for ((*i)++; s[*i] && s[*i] != '\''; (*i)++)
            field_literal(f, s[*i]);
        *i += s[*i] ? 1 : 0;
        return 0;
    }
    if (c == '"')
        return expand_dquote(s, i, f);
    if (c == '$' || c == '`')
        return expand_unquoted(s, i, f);
    str_push(&f->cur, c);
    str_push(&f->pat, c);
    if (c == '*' || c == '?' || c == '[')
        f->has_glob = true;
    (*i)++;
    return 0;
}
