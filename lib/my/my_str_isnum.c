/*
** EPITECH PROJECT, 2024
** isnum
** File description:
** numero
*/

int my_str_isnum(char *str)
{
    int i = 0;

    while (str[i]){
        if (str[i] >= '0' && str[i] <= '9')
            i++;
        else
            return 0;
    }
    return 1;
}
