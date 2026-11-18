/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** variables.h - shell variables (set / unset)
*/

#ifndef VARIABLES_H
    #define VARIABLES_H

typedef struct s_var {
    char *key;
    char *value;
    struct s_var *next;
} var_t;

var_t *find_variable(var_t *vars, const char *key);
void set_variable(var_t **vars, const char *key, const char *value);
void unset_variable(var_t **vars, const char *key);
void free_variables(var_t *vars);

#endif /* VARIABLES_H */
