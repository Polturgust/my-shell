/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** handle_var_builtins.c
*/
#include "variables.h"
#include "mysh.h"
#include "list.h"
#include "my.h"
#include "my.h"

int handle_variable_builtins(char *line, var_t **vars)
{
    char **args = my_str_to_word_array(line, " \t");

    if (args && args[0]) {
        if (my_strcmp(args[0], "set") == 0) {
            builtin_set(args, vars);
            free_tab(args);
            return 1;
        }
        if (my_strcmp(args[0], "unset") == 0) {
            builtin_unset(args, vars);
            free_tab(args);
            return 1;
        }
    }
    free_tab(args);
    return 0;
}
