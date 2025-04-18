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
    char **arr = my_str_to_word_array(argv, "<");
    int fd = 0;
    int in = dup(STDIN_FILENO);
    char **space = my_str_to_word_array(arr[1], " \t");
    int retval = 0;

    if (!arr)
        return 84;
    fd = open(*space, O_RDONLY);
    if (fd == -1)
        return 84;
    dup2(fd, STDIN_FILENO);
    retval = check_alias(arr[0], env);
    free_tab(arr);
    free_tab(space);
    close(fd);
    dup2(in, STDIN_FILENO);
    return retval;
}

char d_droite(char *argv, char **env)
{
    char **arr = my_str_to_word_array(argv, ">");
    int fd = 0;
    int out = dup(STDOUT_FILENO);
    char **space = my_str_to_word_array(arr[1], " \t");
    int retval = 0;

    if (!arr)
        return 84;
    fd = open(*space, O_CREAT | O_RDWR | O_APPEND, 00666);
    dup2(fd, STDOUT_FILENO);
    retval = check_alias(arr[0], env);
    close(fd);
    dup2(out, STDOUT_FILENO);
    free_tab(arr);
    free_tab(space);
    return retval;
}

char rd_droite(char *argv, char **env)
{
    char **arr = my_str_to_word_array(argv, ">");
    int fd = 0;
    int out = dup(STDOUT_FILENO);
    char **space = my_str_to_word_array(arr[1], " \t");
    int retval = 0;

    if (!arr)
        return 84;
    fd = open(*space, O_CREAT | O_RDWR | O_TRUNC, 00666);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    retval = execute_fonc(arr[0], env);
    dup2(out, STDOUT_FILENO);
    free_tab(arr);
    free_tab(space);
    return retval;
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
    char **arr = my_str_to_word_array(line, ";");
    int retval = 0;

    if (!arr)
        return 84;
    for (int i = 0; arr[i]; i++){
        retval = built_pipe(arr[i], env);
    }
    free_tab(arr);
    return retval;
}
