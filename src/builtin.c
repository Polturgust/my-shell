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
    char *builtin[] = {"env", "setenv", "unsetenv", "cd", "42", "alias",
        NULL};
    int (*tab[])(linked_list_t **, char **) =
    {&penv, &etenv, &nsetenv, &set_cd, &karentede, &aliases, NULL};
    char **arr = my_str_to_word_array(arg, " \t");
    int retval = -1;

    if (!arr)
        return 84;
    for (int i = 0; builtin[i]; i++)
        if (my_strcmp(builtin[i], arr[0]) == 0){
            retval = tab[i](create_env(env), arr);
        }
    free_tab(arr);
    return retval;
}
