/*
** EPITECH PROJECT, 2024
** c genial
** File description:
** ça compare sur n caractere
*/

int my_strncmp(char *dest, char *str, int n)
{
    for (int i = 0; i < n; i++){
        if (str[i] - dest[i] != 0)
            return str[i] - dest[i];
    }
    return 0;
}
