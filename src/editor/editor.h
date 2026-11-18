/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** editor.h - state of the line being edited
*/

#ifndef EDITOR_H_
    #define EDITOR_H_
    #include "shell.h"

typedef enum key_result_e {
    K_CONTINUE,
    K_ACCEPT,
    K_EOF
} key_result_t;

typedef struct editor_s {
    str_t buf;
    size_t pos;
    const char *prompt;
    size_t cursor_row;
    long hist_index;
    char *saved;
    str_t kill;
    bool searching;
    str_t query;
    long match;
    char *before_search;
    bool cancelled;
} editor_t;

void editor_refresh(editor_t *e);
size_t display_width(const char *s, size_t n);
key_result_t editor_key(editor_t *e, unsigned char c);
key_result_t editor_escape(editor_t *e);
void edit_insert(editor_t *e, const char *text, size_t n);
void edit_left(editor_t *e);
void edit_right(editor_t *e);
void edit_word_left(editor_t *e);
void edit_word_right(editor_t *e);
void edit_backspace(editor_t *e);
void edit_delete(editor_t *e);
void edit_kill(editor_t *e, size_t from, size_t to);
void edit_yank(editor_t *e);
void edit_set(editor_t *e, const char *text);
void hist_prev(editor_t *e);
void hist_next(editor_t *e);
void search_start(editor_t *e);
bool search_key(editor_t *e, unsigned char c, key_result_t *res);
void complete(editor_t *e);
void complete_candidates(const char *word, bool command, vec_t *out);
void complete_commands(const char *prefix, vec_t *out);
void complete_variables(const char *prefix, vec_t *out);
void print_columns(vec_t *items);
void editor_redraw_all(editor_t *e);

#endif /* EDITOR_H_ */
