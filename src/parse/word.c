/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** word.c - find where a word ends, honouring quotes and substitutions
*/

#include <string.h>
#include "shell.h"

static int skip_until(const char *line, size_t *i, char close, bool escapes)
{
    char open = line[*i];

    for ((*i)++; line[*i] && line[*i] != close; (*i)++)
        if (escapes && line[*i] == '\\' && line[*i + 1])
            (*i)++;
    if (!line[*i]) {
        print_error("Unmatched '%c'.\n", open);
        return -1;
    }
    (*i)++;
    return 0;
}

static int skip_dollar_paren(const char *line, size_t *i)
{
    int depth = 0;

    for (*i += 1; line[*i]; (*i)++) {
        if (line[*i] == '\'' || line[*i] == '"' || line[*i] == '`') {
            if (skip_until(line, i, line[*i], line[*i] != '\'') < 0)
                return -1;
            (*i)--;
            continue;
        }
        depth += (line[*i] == '(') - (line[*i] == ')');
        if (depth == 0) {
            (*i)++;
            return 0;
        }
    }
    print_error("Too many ('s.\n");
    return -1;
}

static int skip_double_quotes(const char *line, size_t *i)
{
    for ((*i)++; line[*i] && line[*i] != '"'; (*i)++) {
        if (line[*i] == '\\' && line[*i + 1]) {
            (*i)++;
            continue;
        }
        if (line[*i] == '$' && line[*i + 1] == '(') {
            if (skip_dollar_paren(line, i) < 0)
                return -1;
            (*i)--;
        }
    }
    if (!line[*i])
        return print_error("Unmatched '\"'.\n") * -1;
    (*i)++;
    return 0;
}

static int skip_one(const char *line, size_t *i, bool *quoted)
{
    char c = line[*i];

    if (c == '\\') {
        *quoted = true;
        *i += line[*i + 1] ? 2 : 1;
        return 0;
    }
    if (c == '\'' || c == '`')
        *quoted = *quoted || c == '\'';
    if (c == '\'' || c == '`')
        return skip_until(line, i, c, c == '`');
    if (c == '"') {
        *quoted = true;
        return skip_double_quotes(line, i);
    }
    if (c == '$' && line[*i + 1] == '(')
        return skip_dollar_paren(line, i);
    if (c == '$' && line[*i + 1] == '{')
        return skip_until(line, i, '}', false);
    (*i)++;
    return 0;
}

int scan_word_end(const char *line, size_t *i, bool *quoted)
{
    *quoted = false;
    while (line[*i] && !strchr(" \t\n|&;()<>", line[*i]))
        if (skip_one(line, i, quoted) < 0)
            return -1;
    return 0;
}
