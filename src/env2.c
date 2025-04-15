/*
** EPITECH PROJECT, 2025
** env.c
** File description:
** env.c
*/

#include "list.h"
#include "mysh.h"

char str_alphanum(char *str)
{
    if (!str)
        return 84;
    for (int i = 0; str[i]; i++){
        if (((str[i] < 'a' || str[i] > 'z') &&
        (str[i] < 'A' || str[i] > 'Z')) && (str[i] < '0' || str[i] > '9'))
            return 0;
    }
    return 1;
}

static int verifarg(char **arr, linked_list_t **env)
{
    if (!arr[1]){
        penv(env, arr);
        return 84;
    }
    if (len_arr(arr) > 3){
        write(2, "setenv: Too many arguments.\n", 28);
        return 84;
    }
    if ((arr[1][0] < 'a' || arr[1][0] > 'z') &&
        (arr[1][0] < 'A' || arr[1][0] > 'Z')){
        write(2, "setenv: Variable name must begin with a letter.\n", 48);
        return 84;
    }
    if (!str_alphanum(arr[1])){
        write(2,
        "setenv: Variable name must contain alphanumeric characters.\n", 60);
        return 84;
    }
    return 0;
}

int etenv(linked_list_t **env, char **arr)
{
    data_t *data = NULL;

    if (!*env){
        push_front(env, data);
        return 0;
    } else if (verifarg(arr, env) != 0)
        return 84;
    data = malloc(sizeof(data_t));
    data->var = my_strdup(arr[1]);
    data->data = my_strdup(arr[2]);
    for (linked_list_t *i = *env; i != NULL; i = i->next)
        if (my_strcmp(data->var, ((data_t *)i->data)->var) == 0){
            free(((data_t *)i->data)->data);
            ((data_t *)i->data)->data = data->data;
            free(data->var);
            free(data);
            return 0;
        }
    push_back(env, data);
    return 0;
}
