/*
** EPITECH PROJECT, 2024
** B-CPE-110-NCE-1-1-settingup-raphael.dedack
** File description:
** str_to.c
*/

#include "my.h"
#include <stdio.h>

char is_in(char c, char *str)
{
    for (int i = 0; str[i]; i++)
        if (str[i] == c)
            return 1;
    return 0;
}

int str_len_slice(char *str, char *slice)
{
    int cpt = 0;

    for (; !is_in(str[cpt], slice) && str[cpt] != '\0'; cpt++);
    return cpt;
}

int str_to_int(char *str)
{
    int expo = 1;
    int accu = 0;

    if (str[0] > '9' || str[0] < '0')
        return 84;
    for (int i = 0; i < my_strlen(str) - 1; i++)
        expo *= 10;
    for (int j = 0; str[j]; j++){
        accu += (str[j] - 48) * expo;
        expo = expo / 10;
    }
    return accu;
}

int nbr_word(char *str, char *slice)
{
    char verif = 0;
    int accu = 0;

    for (int i = 0; str[i]; i++){
        if (!is_in(str[i], slice) && !verif){
            verif = 1;
            accu++;
        }
        if (is_in(str[i], slice) && verif)
            verif = 0;
    }
    return accu;
}

char **my_str_to_word_array(char *str, char *slice)
{
    char **arr = NULL;
    int k = 0;
    int j = 0;

    if (!str || nbr_word(str, slice) == 0)
        return NULL;
    arr = malloc(sizeof(char *) * (nbr_word(str, slice) + 1));
    for (int i = 0; i < my_strlen(str); i++){
        if (is_in(str[i], slice))
            continue;
        arr[k] = my_malloc(str_len_slice(str + i, slice) + 1);
        my_strncpy(arr[k], str + i, str_len_slice(str + i, slice));
        i += str_len_slice(str + i, slice);
        j = 0;
        k++;
    }
    arr[k] = NULL;
    return arr;
}
