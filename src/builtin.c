/*
** EPITECH PROJECT, 2025
** env.c
** File description:
** env.c
*/

#include "list.h"
#include "my.h"
#include "mysh.h"

int test_builtin(char *arg, char **env)
{
    char *builtin[] = {"env", "setenv", "unsetenv", "cd", "exit", "alias",
        NULL};
    int (*tab[])(linked_list_t **, char **) =
    {&penv, &etenv, &nsetenv, &set_cd, &my_exit, &aliases, NULL};
    char **arr = str_skip_arr(arg, " \t");
    int retval = -1;

    if (!arr)
        return 84;
    for (int i = 0; builtin[i]; i++)
        if (my_strcmp(builtin[i], arr[0]) == 0)
            retval = tab[i](create_env(env), arr);
    free_tab(arr);
    if (retval == -1)
        return -1;
    exit_info()->ret_val = retval;
    return exit_info()->ret_val;
}
