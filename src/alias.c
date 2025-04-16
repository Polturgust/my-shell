/*
** EPITECH PROJECT, 2025
** aliases.c
** File description:
** aliases.c
*/

#include "list.h"
#include "mysh.h"

static data_t *add_value(char *line)
{
    char **arr = my_str_to_word_array(line, "=\"\n");
    char **val = NULL;
    data_t *data = malloc(sizeof(data_t));

    data->data = my_strdup(arr[1]);
    val = my_str_to_word_array(arr[0], " ");
    data->var = my_strdup(val[1]);
    free_tab(val);
    free_tab(arr);
    return data;
}

linked_list_t **get_alias(void)
{
    static linked_list_t *alias = NULL;
    FILE *file = fopen("myshrc", "r");
    char *line = NULL;
    size_t size = 0;

    if (!file)
        return &alias;
    if (alias != NULL)
        return &alias;
    while (getline(&line, &size, file) != -1)
        if (my_strncmp("alias ", line, 6) == 0)
            push_front(&alias, add_value(line));
    return &alias;
}

int check_alias(char *arg, char **env)
{
    char **arr = my_str_to_word_array(arg, " ");

    for (linked_list_t *i = *get_alias(); i; i = i->next){
        printf("%s\n", arr[0]);
        if (my_strcmp(arr[0], ((data_t *)i->data)->var) == 0){
            free(arr[0]);
            arr[0] = my_strdup(((data_t *)i->data)->data);
        }
    }
    free(arg);
    arg = arr_to_str(arr, ' ');
    free_tab(arr);
    return execcommand(arg, env);
}

char *arr_to_str(char **arr, char sep)
{
    int accu = 0;
    char *str = NULL;
    int count = 0;

    for (int i = 0; arr[i]; i++){
        accu += my_strlen(arr[i]);
        accu++;
    }
    str = malloc(accu);
    for (int i = 0; arr[i]; i++){
        my_strcpy(str + count, arr[i]);
        count += my_strlen(arr[i]);
        str[count] = sep;
        count++;
    }
    str[count - 1] = '\0';
    return str;
}

int aliases(__attribute__((unused)) linked_list_t **list, char **arr)
{
    char *line_alias = arr_to_str(arr, ' ');

    push_front(get_alias(), add_value(line_alias));
    free(line_alias);
    return 0;
}
