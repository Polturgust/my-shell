/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** expand.c - !!, !n, !-n, !prefix, !$ and !*
*/

#include <stdlib.h>
#include <string.h>
#include "shell.h"

static const char *find_event(const char *spec, size_t len)
{
    size_t n = history_len();
    long num = 0;

    if (n == 0)
        return NULL;
    if (len == 1 && spec[0] == '!')
        return history_at(n - 1)->cmd;
    if (spec[0] == '-' && len > 1) {
        num = atol(spec + 1);
        return num > 0 && (size_t)num <= n ? history_at(n - num)->cmd : NULL;
    }
    if (spec[0] >= '0' && spec[0] <= '9') {
        num = atol(spec);
        for (size_t i = 0; i < n; i++)
            if (history_at(i)->id == num)
                return history_at(i)->cmd;
        return NULL;
    }
    for (size_t i = n; i > 0; i--)
        if (strncmp(history_at(i - 1)->cmd, spec, len) == 0)
            return history_at(i - 1)->cmd;
    return NULL;
}

static void append_words(str_t *out, const char *cmd, bool last_only)
{
    const char *start = cmd + strspn(cmd, " \t");
    const char *last = start;
    const char *args = start + strcspn(start, " \t");

    for (const char *p = start; *p; p++)
        if ((*p == ' ' || *p == '\t') && p[1] && p[1] != ' ' && p[1] != '\t')
            last = p + 1;
    if (last_only) {
        str_appendn(out, last, strcspn(last, " \t"));
        return;
    }
    str_append(out, args + strspn(args, " \t"));
}

static size_t spec_length(const char *s)
{
    size_t len = 0;

    if (s[0] == '!' || s[0] == '$' || s[0] == '*')
        return 1;
    if (s[0] == '-')
        len = 1;
    while (s[len] && !strchr(" \t;|&()<>\"'`", s[len]))
        len++;
    return len;
}

static int expand_bang(const char *line, size_t *i, str_t *out)
{
    const char *spec = line + *i + 1;
    size_t len = spec_length(spec);
    const char *event = NULL;

    if (spec[0] == '$' || spec[0] == '*')
        event = history_len() ? history_at(history_len() - 1)->cmd : NULL;
    else
        event = find_event(spec, len);
    if (!event) {
        print_error("%.*s: Event not found.\n", (int)len, spec);
        return -1;
    }
    if (spec[0] == '$' || spec[0] == '*')
        append_words(out, event, spec[0] == '$');
    else
        str_append(out, event);
    *i += len + 1;
    return 0;
}

int history_expand(const char *line, char **result)
{
    str_t out;
    bool changed = false;
    bool squote = false;

    str_init(&out);
    for (size_t i = 0; line[i];) {
        squote = line[i] == '\'' ? !squote : squote;
        if (line[i] == '\\' && line[i + 1] == '!') {
            str_push(&out, '!');
            i += 2;
            continue;
        }
        if (squote || line[i] != '!' || !line[i + 1]
            || strchr(" \t\n=(", line[i + 1])) {
            str_push(&out, line[i++]);
            continue;
        }
        if (expand_bang(line, &i, &out) < 0) {
            str_free(&out);
            return -1;
        }
        changed = true;
    }
    *result = str_take(&out);
    return changed;
}
