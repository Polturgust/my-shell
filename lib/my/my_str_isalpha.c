/*
** EPITECH PROJECT, 2024
** alpha
** File description:
** oui
*/

int my_str_isalpha(char *str)
{
    int i = 0;

    while (str[i] != '\0'){
        if ((str[i] >= 'a' && str[i] <= 'z') ||
            (str[i] >= 'A' && 'Z' >= str[i]))
            i++;
        else
            return 0;
    }
    return 1;
}
