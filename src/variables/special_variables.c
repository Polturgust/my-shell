/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** special_variables.c
*/
#include "variables.h"

void init_special_variables(var_t **vars)
{
    char *term = getenv("TERM");
    char path[PATH_MAX];

    if (term)
        set_variable(vars, "TERM", term);
    if (getcwd(path, sizeof(path)) != NULL)
        set_variable(vars, "CWD", path);
}

void update_cwd(var_t **vars)
{
    char path[PATH_MAX];
    var_t *cwdcmd;

    if (getcwd(path, sizeof(path)) != NULL)
        set_variable(vars, "CWD", path);
    cwdcmd = find_variable(*vars, "cwdcmd");
    if (cwdcmd && cwdcmd->value)
        system(cwdcmd->value);
}

void run_precmd(var_t *vars)
{
    var_t *precmd = find_variable(vars, "precmd");

    if (precmd && precmd->value)
        system(precmd->value);
}
