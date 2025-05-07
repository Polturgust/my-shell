/*
** EPITECH PROJECT, 2025
** backticks.c
** File description:
** backticks.c
*/

#include "mysh.h"

int test_line(char *line)
{
    int i = 1;

    if (line[0] != '`' && is_in('`', line))
        return 84;
    if (line[0] != '`')
        return 0;
    for (; line[i] && line[i] != '`'; i++);
    if (i > my_strlen(line))
        return 84,
    return 1;   
}

int exec_backticks(char **arr, int i)
{
    char *fonc = NULL;
    int j = 1;

    for (;arr[i][j] && arr[i][j] != '`'; j++);
    fonc = strndup(arr[i] + j, j - 2);
    
}

int backticks(char *line, char **env)
{
    char **arr = str_skip_arr(line, " ");

    if (!arr)
        return 84;
    for (int i = 0; arr[i]; i++){
        if (test_line(arr[i]) == 84){
            free_tab(arr);
            return 84;
        }
        if (test_line(arr[i]))
            exec_backticks(arr, i);
    }    
}
