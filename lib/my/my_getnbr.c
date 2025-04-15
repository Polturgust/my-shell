/*
** EPITECH PROJECT, 2024
** put nbr
** File description:
** number
*/

#include "my.h"

int my_getnbr(char const *str)
{
    int i = 0;
    long c = 1;
    long n = 0;

    while (str[i] && ((str[i] >= 48 && str[i] < 58)
        || str[i] == '-' || str[i] == '+')){
        if (str[i] == '-')
            c = c * -1;
        if (str[i] < 58 && str[i] >= 48)
            n = n * 10 + (str[i] - 48);
        i++;
        if (n * c > 2147483647 || n * c < -2147483648)
            return 0;
    }
    return n * c;
}
