/*
** EPITECH PROJECT, 2024
** lowcase
** File description:
** miniscule
*/

void my_strlowcase(char *str)
{
    for (int i = 0; str[i]; i++)
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32;
}
