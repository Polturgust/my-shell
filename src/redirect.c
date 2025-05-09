/*
** EPITECH PROJECT, 2025
** redirect.c
** File description:
** redirect.c
*/

#include "mysh.h"
#include <fcntl.h>
#include <unistd.h>

char rd_gauche(char *argv, char **env)
{
    char **arr = str_skip_arr(argv, "<");
    int fd = 0;
    int in = dup(STDIN_FILENO);
    char **space = str_skip_arr(arr[1], " \t");

    if (!arr)
        return 84;
    fd = open(*space, O_RDONLY);
    if (fd == -1)
        return 84;
    dup2(fd, STDIN_FILENO);
    exit_info()->ret_val = check_alias(arr[0], env);
    free_tab(arr);
    free_tab(space);
    close(fd);
    dup2(in, STDIN_FILENO);
    return exit_info()->ret_val;
}

char d_droite(char *argv, char **env)
{
    char **arr = str_skip_arr(argv, ">");
    int fd = 0;
    int out = dup(STDOUT_FILENO);
    char **space = str_skip_arr(arr[1], " \t");

    printf("%s\n", *space);
    if (!arr)
        return 84;
    fd = open(*space, O_CREAT | O_RDWR | O_APPEND, 00666);
    dup2(fd, STDOUT_FILENO);
    exit_info()->ret_val = check_alias(arr[0], env);
    close(fd);
    dup2(out, STDOUT_FILENO);
    free_tab(arr);
    free_tab(space);
    return exit_info()->ret_val;
}

char rd_droite(char *argv, char **env)
{
    char **arr = str_skip_arr(argv, ">");
    int fd = 0;
    int out = dup(STDOUT_FILENO);
    char **space = str_skip_arr(arr[1], " \t");

    if (!arr || !space)
        return 84;
    fd = open(*space, O_CREAT | O_RDWR | O_TRUNC, 00666);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    exit_info()->ret_val = check_alias(arr[0], env);
    dup2(out, STDOUT_FILENO);
    free_tab(arr);
    free_tab(space);
    return exit_info()->ret_val;
}

char test_redir(char *argv, char **env)
{
    char (*red[])(char *, char **) = {&rd_gauche, &rd_droite, &d_droite, NULL};
    int nb_red[4] = {0};

    for (int i = 0; argv[i]; i++){
        if (my_strncmp(argv + i, ">>", 2) == 0){
            nb_red[2]++;
            continue;
        }
        if (argv[i] == '>')
            nb_red[1]++;
        if (argv[i] == '<')
            nb_red[0]++;
    }
    for (int i = 0; i < 4; i++){
        if (nb_red[2] > 0)
            return red[2](argv, env);
        if (nb_red[i] > 0)
            return red[i](argv, env);
    }
    return check_alias(argv, env);
}

char test_semi(char *line, char **env)
{
    char **arr = str_skip_arr(line, ";");

    if (!arr)
        return 84;
    for (int i = 0; arr[i]; i++)
        exit_info()->ret_val = my_and(arr[i], env);
    free_tab(arr);
    return exit_info()->ret_val;
}
