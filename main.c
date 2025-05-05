/*
** EPITECH PROJECT, 2024
** B-PSU-200-NCE-2-1-42sh-florent.serra
** File description:
** main.c
*/

#include "include/mysh.h"
#include "include/list.h"
#include "include/my.h"
#include "lib/my/my.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv, char **env)
{
    int ret = 0;

    if (argc == 2)
        if (my_strcmp(argv[1], "-h") == 0){
            write(1, "usage: ./mysh\n", 14);
            return 0;
        }
    get_history();
    ret = minishell(env);
    destroy_env(create_env(env));
    destroy_env(get_alias());
    set_history();
    return ret;
}
