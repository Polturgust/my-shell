/*
** EPITECH PROJECT, 2024
** strdup
** File description:
** duplicate
*/
#include <stdlib.h>
#include "my.h"

char *my_strdup(char *bozo)
{
    char *str;

    if (!bozo)
        return bozo;
    str = malloc(sizeof(char) * (my_strlen(bozo) + 1));
    my_strcpy(str, bozo);
    return str;
}
