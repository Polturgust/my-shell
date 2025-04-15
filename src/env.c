/*
** EPITECH PROJECT, 2025
** env.c
** File description:
** env.c
*/

#include "../include/mysh.h"
#include "list.h"
#include "my.h"

static data_t *fill_struct(char *variable)
{
    data_t *data = malloc(sizeof(data_t));
    int lenvar = str_len_slice(variable, "=");

    data->var = malloc(lenvar + 1);
    if (!data && !data->var)
        return NULL;
    my_strncpy(data->var, variable, lenvar);
    data->var[lenvar] = '\0';
    data->data = my_strdup(variable + lenvar + 1);
    return data;
}

static void free_data(data_t *data)
{
    free(data->var);
    free(data->data);
    free(data);
}

static int test_list(linked_list_t *list, char *arr, int *idc)
{
    int val = 0;

    val = my_strcmp(arr, ((data_t *)list->data)->var);
    for (linked_list_t *i = list->next; i && val != 0; i = i->next){
        *idc += 1;
        val = my_strcmp(arr, ((data_t *)i->data)->var);
    }
    return val;
}

int nsetenv(linked_list_t **list, char **arr)
{
    int val = 0;
    data_t *data = NULL;
    int idc = 0;

    if (!arr || !list)
        return 84;
    for (int j = 1; arr[j]; j++){
        val = test_list(*list, arr[j], &idc);
        if (val != 0){
            idc = 0;
            continue;
        }
        data = (data_t *)pop_at_idc(list, idc);
        free_data(data);
        idc = 0;
        return 0;
    }
    return 0;
}

int penv(linked_list_t **list, [[maybe_unused]] char **unused)
{
    if (!list)
        return 0;
    for (linked_list_t *i = *list; i != NULL; i = i->next){
        my_putstr(((data_t *)i->data)->var);
        write(1, "=", 1);
        if (((data_t *)i->data)->data != NULL)
            my_putstr(((data_t *)i->data)->data);
        write(1, "\n", 1);
    }
    return 0;
}

void destroy_env(linked_list_t **env)
{
    data_t *freedata = NULL;

    if (!env)
        return;
    while (*env){
        freedata = (data_t *)pop_front(env);
        free(freedata->var);
        free(freedata->data);
        free(freedata);
    }
}

int len_arr(char **arr)
{
    int i = 0;

    if (!arr)
        return i;
    for (; arr[i]; i++);
    return i;
}

linked_list_t **create_env(char **env)
{
    static linked_list_t *list = NULL;
    data_t *data = NULL;

    if (list != NULL)
        return &list;
    if (!*env && !list){
        data = fill_struct(TCSHENV);
        push_front(&list, data);
    }
    if (!list){
        data = fill_struct(env[0]);
        push_front(&list, data);
        for (int i = 1; env[i]; i++){
            data = fill_struct(env[i]);
            push_back(&list, data);
        }
    }
    return &list;
}
