/*
** EPITECH PROJECT, 2024
** capitalize
** File description:
** premiere lettre de chaque mot en majuscule
*/

#include "my.h"

char *my_strcapitalize(char *minus)
{
    int i = 1;

    my_strlowcase(minus);
    while (minus[i]){
        if (minus[0] >= 'a' && 'z' >= minus[0])
            minus[0] = minus[0] - 32;
        if ((minus[i] <= 'z' && minus[i] >= 'a') &&
            (minus[i - 1] == ' ')){
            minus[i] = minus[i] - 32;
        }
        i++;
    }
    return minus;
}
