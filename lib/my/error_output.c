/*
** EPITECH PROJECT, 2024
** B-PSU-100-NCE-1-1-myls-eric.faure-dunand-bruschi
** File description:
** error_output.c
*/

#include "my.h"

int my_puterror_char(char c)
{
    write(2, &c, 1);
    return 1;
}

int my_puterror_str(char *str)
{
    write(2, str, my_strlen(str));
    return 1;
}
