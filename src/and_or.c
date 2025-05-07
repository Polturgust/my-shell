/*
** EPITECH PROJECT, 2025
** and or
** File description:
** and or
*/

#include "mysh.h"
#include <string.h>

char **create_tab(int *idc, int len_idc, char *line)
{
    char **arr = malloc(sizeof(char *) * (len_idc + 1));

    if (!arr)
        return NULL;
    for (int i = 0; i < len_idc; i++){
        if (i < len_idc - 1)
            arr[i] = strndup(&line[idc[i]], idc[i + 1] - idc[i] - 2);
        else
            arr[i] = strdup(&line[idc[i]]);
    }
    arr[len_idc] = NULL;
    return arr;
}

static void cpy_last_line(char ***arr, int len_idc, int idc, char *line)
{
    *arr = realloc(*arr, sizeof(char *) * (len_idc + 1));
    (*arr)[len_idc - 1] = strdup(&line[idc]);
    (*arr)[len_idc] = NULL;
}

int my_or(char *line, char **env)
{
    int idc = 0;
    int len_idc = 1;
    char **arr = NULL;
    int retval = 1;

    for (int i = 0; line[i]; i++)
        if (my_strncmp(&line[i], "||", 2) == 0){
            arr = realloc(arr, sizeof(char *) * (len_idc + 1));
            arr[len_idc - 1] = strndup(&line[idc], i - idc);
            len_idc++;
            i++;
            idc = i + 2;
        }
    if (len_idc == 1)
        return built_pipe(line, env);
    cpy_last_line(&arr, len_idc, idc, line);
    for (int j = 0; arr[j] && retval != 0; j++)
        retval = built_pipe(arr[j], env);
    free_tab(arr);
    return retval;
}

int my_and(char *line, char **env)
{
    int idc = 0;
    int len_idc = 1;
    char **arr = NULL;
    int retval = 0;

    for (int i = 0; line[i]; i++)
        if (my_strncmp(&line[i], "&&", 2) == 0){
            arr = realloc(arr, sizeof(char *) * (len_idc + 1));
            arr[len_idc - 1] = strndup(&line[idc], i - idc);
            len_idc++;
            i++;
            idc = i + 2;
        }
    if (len_idc == 1)
        return my_or(line, env);
    cpy_last_line(&arr, len_idc, idc, line);
    for (int j = 0; arr[j] && retval == 0; j++)
        retval = my_or(arr[j], env);
    free_tab(arr);
    return retval;
}
