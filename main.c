/*
** EPITECH PROJECT, 2024
** B-PSU-200-NCE-2-1-42sh-florent.serra
** File description:
** main.c
*/

#include "include/mysh.h"


int main(int argc, char **argv, char **env)
{
    int ret = 0;
    linked_list_t *i = get_skip();

    if (argc == 2)
        if (my_strcmp(argv[1], "-h") == 0){
            write(1, "usage: ./mysh\n", 14);
            return 0;
        }
    init_history();
    ret = minishell(env);
    destroy_env(create_env(env));
    destroy_env(get_alias());
    remove_history();
    while (i)
        free(pop_front(&i));
    free(exit_info());
    return ret;
}
