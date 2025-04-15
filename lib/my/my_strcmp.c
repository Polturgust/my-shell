/*
** EPITECH PROJECT, 2024
** cmp
** File description:
** str cmp
*/

#include "my.h"

int my_strcmp(char *s1, char *s2)
{
    if (my_strlen(s1) != my_strlen(s2))
        return 84;
    for (int i = 0; s1[i] != '\0'; i++){
        if (s1[i] - s2[i] < 0)
            return s1[i] - s2[i];
        if (s1[i] - s2[i] > 0)
            return s1[i] - s2[i];
    }
    return 0;
}
