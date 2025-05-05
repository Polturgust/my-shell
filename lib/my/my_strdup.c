/*
** EPITECH PROJECT, 2024
** strdup
** File description:
** duplicate
*/
#include <stdlib.h>
#include "my.h"

char *my_strdup(char *str)
{
    char *res;

    if (!str)
        return str;
    res = malloc(sizeof(char) * (my_strlen(str) + 1));
    my_strcpy(res, str);
    return res;
}
