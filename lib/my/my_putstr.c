/*
** EPITECH PROJECT, 2024
** put str
** File description:
** str putting
*/

#include "my.h"

int my_putstr(char const *str)
{
    write(1, str, my_strlen(str));
    return 0;
}
