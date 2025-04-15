/*
** EPITECH PROJECT, 2024
** print nbr
** File description:
** bah ça renvoie un ptn de nombre
*/

#include <stdio.h>
#include "my.h"

int my_put_nbr(long n)
{
    if (n == -2147483648){
        write(1, "-2147483648", 11);
        return 0;
    }
    if (n < 0){
        write(1, "-", 1);
        my_put_nbr(n * -1);
        return 0;
    }
    if (n < 10){
        my_putchar(n + 48);
        return 0;
    } else {
        my_put_nbr(n / 10);
        my_putchar(n % 10 + 48);
        return 0;
    }
}
