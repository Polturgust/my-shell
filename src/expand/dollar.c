/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** dollar.c - $name, ${name}, $?, $$ and $(command)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "field.h"

int find_closing(const char *s, size_t open, char close)
{
    int depth = 0;
    char quote = 0;

    for (size_t i = open; s[i]; i++) {
        if (quote) {
            quote = s[i] == quote ? 0 : quote;
            continue;
        }
        if (s[i] == '\'' || s[i] == '"' || s[i] == '`') {
            quote = s[i];
            continue;
        }
        depth += (s[i] == s[open]) - (s[i] == close);
        if (depth == 0)
            return (int)i;
    }
    return -1;
}

static const char *lookup(const char *name)
{
    static char number[32];
    var_t *var = find_variable(sh()->vars, name);

    if (strcmp(name, "?") == 0 || strcmp(name, "status") == 0) {
        snprintf(number, sizeof(number), "%d", sh()->status);
        return number;
    }
    if (strcmp(name, "$") == 0) {
        snprintf(number, sizeof(number), "%d", (int)getpid());
        return number;
    }
    if (var)
        return var->value;
    return env_get(name);
}

static int append_variable(const char *name, str_t *out)
{
    const char *value = lookup(name);

    if (!value) {
        print_error("%s: Undefined variable.\n", name);
        return -1;
    }
    str_append(out, value);
    return 0;
}

static int dollar_subst(const char *s, size_t *i, str_t *out)
{
    int close = find_closing(s, *i + 1, ')');
    char *cmd = NULL;
    char *result = NULL;

    if (close < 0) {
        print_error("Too many ('s.\n");
        return -1;
    }
    cmd = xstrndup(s + *i + 2, close - *i - 2);
    result = command_substitution(cmd);
    str_append(out, result);
    free(cmd);
    free(result);
    *i = close + 1;
    return 0;
}

int expand_dollar(const char *s, size_t *i, str_t *out)
{
    size_t start = *i + 1;
    size_t end = start;
    char *name = NULL;
    int ret = 0;

    if (s[start] == '(')
        return dollar_subst(s, i, out);
    if (s[start] == '{') {
        for (end = start + 1; s[end] && s[end] != '}'; end++);
        name = xstrndup(s + start + 1, end - start - 1);
        *i = s[end] ? end + 1 : end;
    } else if (s[start] == '?' || s[start] == '$') {
        name = xstrndup(s + start, 1);
        *i = start + 1;
    } else {
        for (end = start; is_name_char(s[end], end == start); end++);
        if (end == start) {
            str_push(out, '$');
            *i = start;
            return 0;
        }
        name = xstrndup(s + start, end - start);
        *i = end;
    }
    ret = append_variable(name, out);
    free(name);
    return ret;
}
