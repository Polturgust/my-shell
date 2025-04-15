/*
** EPITECH PROJECT, 2024
** B-CPE-110-NCE-1-1-organized-raphael.dedack
** File description:
** my_malloc.c
*/

#include "my.h"

void *my_malloc(int size)
{
    void *bozo;

    bozo = malloc(size);
    for (int i = 0; i < size; i++) {
        ((char *)bozo)[i] = 0;
    }
    return bozo;
}
