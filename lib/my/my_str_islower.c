/*
** EPITECH PROJECT, 2024
** lower
** File description:
** oui
*/

int my_str_islower(char *str)
{
    int i = 0;

    while (str[i]){
        if (str[i] >= 'a' && str[i] <= 'z')
            i++;
        else
            return 0;
    }
    return 1;
}
