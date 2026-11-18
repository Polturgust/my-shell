/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** expand_word.c - entry points of the expansion
*/

#include <stdlib.h>
#include <string.h>
#include "field.h"


static void field_init(field_t *f, vec_t *out)
{
    str_init(&f->cur);
    str_init(&f->pat);
    f->has_glob = false;
    f->started = false;
    f->out = out;
}

static void field_free(field_t *f)
{
    str_free(&f->cur);
    str_free(&f->pat);
}

int expand_word(const char *word, vec_t *out)
{
    field_t f;
    size_t i = 0;
    size_t consumed = 0;
    char *home = NULL;
    int ret = 0;

    field_init(&f, out);
    if (word[0] == '~') {
        home = expand_tilde(word, &consumed);
        if (home)
            field_literals(&f, home);
        i = home ? consumed : 0;
        free(home);
    }
    while (word[i] && ret == 0)
        ret = expand_char(word, &i, &f);
    if (ret == 0)
        ret = field_end(&f);
    field_free(&f);
    return ret;
}

int expand_words(const vec_t *words, vec_t *out)
{
    int ret = 0;

    for (size_t i = 0; i < words->len; i++) {
        ret = expand_word(words->items[i], out);
        if (ret == -2)
            print_error("%s: No match.\n",
                out->len ? out->items[0] : words->items[0]);
        if (ret < 0)
            return -1;
    }
    return 0;
}

char *expand_single(const char *word)
{
    vec_t out;
    char *result = NULL;
    int ret = 0;

    vec_init(&out);
    ret = expand_word(word, &out);
    if (ret == -2)
        print_error("%s: No match.\n", word);
    if (ret == 0 && out.len != 1)
        print_error("%s: Ambiguous.\n", word);
    if (ret == 0 && out.len == 1)
        result = xstrdup(out.items[0]);
    vec_free(&out);
    return result;
}
