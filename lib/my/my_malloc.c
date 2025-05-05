/*
** EPITECH PROJECT, 2024
** B-CPE-110-NCE-1-1-organized-raphael.dedack
** File description:
** my_malloc.c
*/

#include "my.h"

void *my_malloc(int size)
{
    void *to_malloc;

    to_malloc = malloc(size);
    for (int i = 0; i < size; i++) {
        ((char *)to_malloc)[i] = 0;
    }
    return to_malloc;
}
