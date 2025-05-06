/*
** EPITECH PROJECT, 2025
** parenthese.c
** File description:
** parenthese.c
*/

#include "../include/mysh.h"
#include <stdlib.h>

typedef struct skip_s {
    char sep;
    char sep_fin;
    int count;
} skip_t;

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

linked_list_t *get_skip()
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
        if (((skip_t *)i->data)->sep == c){
            printf("oui %c\n", c);
            ((skip_t *)i->data)->count++;
            return 1;
        }
        if (((skip_t *)i->data)->sep_fin == c){
            ((skip_t *)i->data)->count--;
            printf("je suis le chimpanzé sacrée %c %i\n", c, ((skip_t *)i->data)->count);
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
        printf("%i\n", verif);
        for (linked_list_t *j = get_skip(); j && verif == 0; j = j->next){
            verif = ((skip_t *)j->data)->count;
        }
        if (verif == 0)
            printf("verif vaut zero\n");
    }
    if (verif != 0){
        printf("verif ne vaut pas 0\n");
        return 1;
    }
    printf("\n");
    printf("%i\n", *count);
    return 0;
}

static int malloc_arr(char *str, int idc, char *slice)
{
    int count = 0;

    for (; str[idc + count] && !is_in(str[idc + count], slice); count++)
        if (is_skip(str[idc + count]))
            skip_str(str, &count, idc);
    return count;
}

char **str_skip_arr(char *str, char *slice)
{
    char **arr = NULL;
    int j = 0;

    for (int i = 0; str[i]; i += malloc_arr(str, i, slice)){
        arr = realloc(arr, sizeof(char *) * (j + 1));
        arr[j] = malloc(malloc_arr(str, i, slice) + 1);
        if (is_in(str[i], slice))
            continue;
        my_strncpy(arr[j], str + i, malloc_arr(str, i, slice));
        //printf("%i %s\n", malloc_arr(str, i, slice), arr[j]);
        j++;
    }
    arr[j] = NULL;
    return arr;
}

int main(void)
{
    char **arr = str_skip_arr("j'aime le caca (oui le | caca) | non", "|");

    if (!arr)
        printf("caca\n");
    for (int i = 0; arr[i]; i++)
        printf("%i %s\n", i, arr[i]);
}
