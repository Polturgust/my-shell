/*
** EPITECH PROJECT, 2024
** put str
** File description:
** str putting
*/

int my_strlen(char const *str)
{
    int i = 0;

    for (; str[i]; i++);
    return i;
}
