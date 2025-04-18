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
    char **arr = my_str_to_word_array(line, " =\"\n");
    char **val = NULL;
    data_t *data = malloc(sizeof(data_t));

    data->data = arr_to_str(&arr[2], ' ');
    data->var = my_strdup(arr[1]);
    free_tab(val);
    free_tab(arr);
    return data;
}

static int parse_alias(char *line)
{
    char **tmp = my_str_to_word_array(line, "\" ");

    if (len_arr(tmp) < 3){
        free_tab(tmp);
        return 1;
    }
    return 0;
}

int test_alias(char *line)
{
    char **arr = my_str_to_word_array(line, " ");
    data_t *data = NULL;

    if (parse_alias(line)){
        free_tab(arr);
        return 0;
    }
    for (linked_list_t *i = *get_alias(); i; i = i->next){
        data = (data_t *)i->data;
        if (my_strcmp(arr[1], data->var) == 0){
            free(data->data);
            free(data->var);
            free(data);
            i->data = add_value(line);
            return 2;
        }
    }
    free_tab(arr);
    push_front(get_alias(), add_value(line));
    return 1;
}

linked_list_t **get_alias(void)
{
    static linked_list_t *alias = NULL;
    FILE *file = fopen(".myshrc", "r");
    char *line = NULL;
    size_t size = 0;

    if (!file)
        return &alias;
    while (getline(&line, &size, file) != -1)
        if (my_strncmp("alias ", line, 6) == 0 && !parse_alias(line))
            push_front(&alias, add_value(line));
    fclose(file);
    return &alias;
}

int check_alias(char *arg, char **env)
{
    char **arr = my_str_to_word_array(arg, " ");
    int retval = 0;

    for (linked_list_t *i = *get_alias(); i; i = i->next){
        if (my_strcmp(arr[0], ((data_t *)i->data)->var) == 0){
            free(arr[0]);
            arr[0] = my_strdup(((data_t *)i->data)->data);
            arg = arr_to_str(arr, ' ');
            free_tab(arr);
            retval = execute_fonc(arg, env);
            free(arg);
            return retval;
        }
    }
    free_tab(arr);
    return execute_fonc(arg, env);
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

    if (!test_alias(line_alias)){
        free(line_alias);
        return 84;
    }
    free(line_alias);
    return 0;
}
