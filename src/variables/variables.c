/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** variables.c
*/
#include "variables.h"

var_t *find_variable(var_t *vars, const char *key)
{
    while (vars) {
        if (strcmp(vars->key, key) == 0)
            return vars;
        vars = vars->next;
    }
    return NULL;
}

void set_variable(var_t **vars, const char *key, const char *value)
{
    var_t *var = find_variable(*vars, key);
    var_t *new_var;

    if (var) {
        free(var->value);
        var->value = strdup(value);
    } else {
        new_var = malloc(sizeof(var_t));
        new_var->key = strdup(key);
        new_var->value = strdup(value);
        new_var->next = *vars;
        *vars = new_var;
    }
}

static void free_var_node(var_t *node)
{
    free(node->key);
    free(node->value);
    free(node);
}

void unset_variable(var_t **vars, const char *key)
{
    var_t *tmp = *vars;
    var_t *prev = NULL;

    while (tmp) {
        if (strcmp(tmp->key, key) != 0) {
            prev = tmp;
            tmp = tmp->next;
            continue;
        }
        if (prev)
            prev->next = tmp->next;
        else
            *vars = tmp->next;
        free_var_node(tmp);
        return;
    }
}

void print_variables(var_t *vars)
{
    while (vars) {
        printf("%s=%s\n", vars->key, vars->value);
        vars = vars->next;
    }
}

void free_variables(var_t *vars)
{
    var_t *tmp;

    while (vars) {
        tmp = vars;
        vars = vars->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}
