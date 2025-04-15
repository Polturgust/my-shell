/*
** EPITECH PROJECT, 2024
** cpy str
** File description:
** bah ça copie
*/

#include "my.h"

char *my_strconcat(char *str1, char *str2)
{
    int len1 = my_strlen(str1);
    int len2 = my_strlen(str2);
    char *concat = malloc(len1 + len2 + 1);

    for (int i = 0; i < len1 + len2; i++)
        if (i < len1)
            concat[i] = str1[i];
        else
            concat[i] = str2[i - len1];
    concat[len1 + len2] = '\0';
    return concat;
}
