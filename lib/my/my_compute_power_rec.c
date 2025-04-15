/*
** EPITECH PROJECT, 2024
** power rec
** File description:
** power
*/

#include "my.h"

int my_compute_power_rec(int n, int e)
{
    long oui = 0;
    long *ptr;

    ptr = &oui;
    *ptr = n * *ptr;
    if (*ptr > 2147483647)
        return 0;
    if (e < 0)
        return 0;
    if (e == 0)
        return 1;
    else
        return n * my_compute_power_rec(n, e - 1);
}
