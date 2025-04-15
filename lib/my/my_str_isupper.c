/*
** EPITECH PROJECT, 2024
** upper
** File description:
** oui
*/

int my_str_isupper(char *str)
{
    for (int i = 0; str[i]; i++)
        if (str[i] > 'Z' || str[i] < 'A')
            return 1;
    return 0;
}
