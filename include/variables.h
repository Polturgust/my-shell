/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** variables.h
*/
#ifndef VARIABLES_H
    #define VARIABLES_H
    # define PATH_MAX 4096
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <limits.h>

typedef struct s_var {
    char *key;
    char *value;
    struct s_var *next;
} var_t;

var_t *find_variable(var_t *vars, const char *key);
void set_variable(var_t **vars, const char *key, const char *value);
void unset_variable(var_t **vars, const char *key);
void print_variables(var_t *vars);
void free_variables(var_t *vars);

int builtin_set(char **args, var_t **vars);
int builtin_unset(char **args, var_t **vars);

void init_special_variables(var_t **vars);
void update_cwd(var_t **vars);
void run_precmd(var_t *vars);
int handle_eof(var_t *vars);


#endif /* VARIABLES_H */
