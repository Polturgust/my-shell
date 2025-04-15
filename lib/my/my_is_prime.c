/*
** EPITECH PROJECT, 2024
** premier
** File description:
** trouve si le nbr est premier
*/

int my_is_prime(int n)
{
    if (n < 2)
        return 0;
    for (int i = 2; i * i < n; i++){
        if (n % i == 0)
            return 0;
    }
    return 1;
}
