/*
** EPITECH PROJECT, 2024
** majuscule
** File description:
** oui
*/

void my_strupcase(char *str)
{
    for (int i = 0; str[i]; i++)
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] = str[i] - 32;
}
