/*
** EPITECH PROJECT, 2024
** prime sup
** File description:
** le premier du dessus
*/

#include "my.h"

int my_find_prime_sup(int n)
{
    int p = my_is_prime(n);

    while (p != 1){
        n++;
        p = my_is_prime(n);
    }
    return n;
}
