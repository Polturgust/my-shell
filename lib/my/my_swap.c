/*
** EPITECH PROJECT, 2024
** my_swap
** File description:
** swap 2 valeurs
*/

int my_swap(int *a, int *b)
{
    int c = *a;

    *a = *b;
    *b = c;
    return 0;
}
