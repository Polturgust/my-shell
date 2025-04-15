/*
** EPITECH PROJECT, 2024
** square
** File description:
** racine carre
*/

#include "my.h"

int my_compute_square_root(int n)
{
    for (int i = 1; i * i < n; i++){
        if (i * i == n)
            return i;
    }
    return 0;
}
