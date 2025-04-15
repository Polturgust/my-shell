/*
** EPITECH PROJECT, 2024
** strstr
** File description:
** concatenation
*/

#include "my.h"

char *my_strstr(char *str, char const *to_find)
{
    int j = my_strlen(to_find);
    int i = 0;
    int g = 1;

    while (str[i]){
        if (str[i] - to_find[0] == 0)
            g = my_strncmp(&str[i], to_find, j);
        if (g == 0) {
            return &str[i];
    }
        i++;
    }
    return 0;
}
