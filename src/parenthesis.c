/*
** EPITECH PROJECT, 2025
** parenthese.c
** File description:
** parenthese.c
*/

#include "../include/mysh.h"
#include <stdlib.h>

skip_t *create_skip(char *line)
{
    skip_t *skip = malloc(sizeof(skip_t));

    if (!skip)
        return NULL;
    skip->sep = line[0];
    skip->sep_fin = line[1];
    skip->count = 0;
    return skip;
}

linked_list_t *get_skip(void)
{
    static linked_list_t *skip = NULL;
    char *arr[] = {"()", "\"\"", "[]", "“”", NULL};

    if (!skip)
        for (int i = 0; arr[i]; i++)
            push_back(&skip, create_skip(arr[i]));
    return skip;
}

char is_skip(char c)
{
    for (linked_list_t *i = get_skip(); i; i = i->next){
        if (((skip_t *)i->data)->sep_fin == ((skip_t *)i->data)->sep &&
            ((skip_t *)i->data)->count == 1 && ((skip_t *)i->data)->sep == c){
            ((skip_t *)i->data)->count--;
            return 1;
        }
        if (((skip_t *)i->data)->sep == c){
            ((skip_t *)i->data)->count++;
            return 1;
        }
        if (((skip_t *)i->data)->sep_fin == c){
            ((skip_t *)i->data)->count--;
            return 1;
        }
    }
    return 0;
}

int skip_str(char *line, int *count, int idc)
{
    int verif = ((skip_t *)get_skip()->data)->count;

    *count += 1;
    for (; line[*count + idc]; *count += 1){
        is_skip(line[*count + idc]);
        verif = ((skip_t *)get_skip()->data)->count;
        for (linked_list_t *j = get_skip(); j && verif == 0; j = j->next){
            verif = ((skip_t *)j->data)->count;
        }
        if (verif == 0)
            break;
    }
    if (verif != 0)
        return 1;
    return 0;
}

static int malloc_arr(char *str, int idc, char *slice)
{
    int count = 0;

    if (idc > my_strlen(str))
        return count;
    for (; str[idc + count] && !is_in(str[idc + count], slice); count++){
        if (is_skip(str[idc + count]))
            skip_str(str, &count, idc);
    }
    return count;
}

char **str_skip_arr(char *str, char *slice)
{
    char **arr = NULL;
    int j = 0;
    int size = 0;

    for (int i = 0; i < my_strlen(str); i++){
        size = malloc_arr(str, i, slice);
        if (size == 0)
            break;
        arr = realloc(arr, sizeof(char *) * (j + 1));
        arr[j] = malloc(size + 1);
        if (is_in(str[i], slice))
            continue;
        strncpy(arr[j], str + i, size);
        arr[j][size] = '\0';
        i += size;
        j++;
    }
    arr = realloc(arr, sizeof(char *) * (j + 1));
    arr[j] = NULL;
    return arr;
}
