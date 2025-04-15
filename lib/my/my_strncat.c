/*
** EPITECH PROJECT, 2024
** str cat
** File description:
** sur n
*/

#include "my.h"

char *my_strncat(char *dest, char const *src, int n)
{
    int i = my_strlen(dest);

    for (int j = 0; j < n; j++){
        dest[i] = src[j];
        i++;
    }
    dest[i] = '\0';
    return dest;
}
