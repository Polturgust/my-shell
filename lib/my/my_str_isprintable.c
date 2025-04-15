/*
** EPITECH PROJECT, 2024
** printable
** File description:
** bah le titre
*/

int my_str_isprintable(char *str)
{
    int i = 0;

    while (str[i]){
        if (str[i] >= 32 && str[i] <= 126)
            i++;
        else
            return 0;
    }
    return 1;
}
