/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** field.h - state used while expanding one word into fields
*/

#ifndef FIELD_H_
    #define FIELD_H_
    #include "shell.h"

typedef struct field_s {
    str_t cur;
    str_t pat;
    bool has_glob;
    bool started;
    vec_t *out;
} field_t;

void field_literal(field_t *f, char c);
void field_literals(field_t *f, const char *text);
void field_split_append(field_t *f, const char *text);
int field_end(field_t *f);
int find_closing(const char *s, size_t open, char close);
int expand_char(const char *s, size_t *i, field_t *f);

#endif /* FIELD_H_ */
