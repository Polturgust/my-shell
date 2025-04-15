/*
** EPITECH PROJECT, 2024
** revstr
** File description:
** str rev
*/

#include "my.h"

char *my_revstr(char *str)
{
    int len = my_strlen(str);
    char *str2 = my_malloc(sizeof(char) * (len + 1));
    int i = 0;

    for (; str[i]; i++){
        str2[i] = str[len - 1];
    len--;
    }
    str2[i] = '\0';
    return str2;
}
