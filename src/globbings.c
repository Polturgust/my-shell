/*
** EPITECH PROJECT, 2024
** B-PSU-200-NCE-2-1-42sh-florent.serra
** File description:
** globbings.c
*/

#include "mysh.h"
#include <glob.h>

int has_globbing(const char *str)
{
    for(int i = 0; str[i]; i++)
        if (str[i] == '*' || str[i] == '?' || str[i] == '[')
        return 1;
    return 0;
}
