/*
** EPITECH PROJECT, 2025
** backticks.c
** File description:
** backticks.c
*/

#include "mysh.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int test_line(char *line)
{
    int i = 1;

    if (line[0] != '`' && is_in('`', line))
        return 84;
    if (line[0] != '`')
        return 0;
    for (; line[i] && line[i] != '`'; i++);
    if (i > my_strlen(line))
        return 84;
    return 1;
}

char *replace_back(void)
{
    FILE *fd = fopen("tmp", "r");
    struct stat size = {0};
    char *buffer = NULL;
    size_t l = 0;
    int i = 0;

    if (!fd)
        return NULL;
    if (stat("tmp", &size) == -1)
        return NULL;
    buffer = malloc(size.st_size + 1);
    getline(&buffer, &l, fd);
    remove("tmp");
    for (; buffer[i] != '\n'; i++);
    buffer[i] = '\0';
    return buffer;
}

int exec_backticks(char **arr, int i, char **env)
{
    char *fonc = NULL;
    char *tmp = NULL;
    int j = 1;

    for (; arr[i][j] && arr[i][j] != '`'; j++);
    tmp = strndup(arr[i] + 1, j - 1);
    fonc = my_strconcat(tmp, "> tmp");
    free(tmp);
    test_redir(fonc, env);
    free(fonc);
    free(arr[i]);
    arr[i] = replace_back();
    return 0;
}

static int bckloop(char **arr, char **env)
{
    for (int i = 0; arr[i]; i++){
        if (test_line(arr[i]) == 84){
            free_tab(arr);
            return 84;
        }
        if (test_line(arr[i]))
            exec_backticks(arr, i, env);
    }
    return 0;
}

int backticks(char *line, char **env)
{
    char **arr = str_skip_arr(line, " ");
    char *new_line = NULL;

    if (!arr)
        return 84;
    if (!is_in('`', line)){
        free_tab(arr);
        return test_semi(line, env);
    }
    bckloop(arr, env);
    new_line = arr_to_str(arr, ' ');
    free_tab(arr);
    exit_info()->ret_val = test_semi(new_line, env);
    free(new_line);
    return exit_info()->ret_val;
}
