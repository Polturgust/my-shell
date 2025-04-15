/*
** EPITECH PROJECT, 2024
** putchar
** File description:
** le vrai
*/

#include "my.h"

void my_putchar(char c)
{
    write(1, &c, 1);
}
