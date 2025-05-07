/*
** EPITECH PROJECT, 2024
** B-PSU-200-NCE-2-1-42sh-florent.serra
** File description:
** history2.c
*/

#include "../include/mysh.h"

linked_list_t **hist_info(void)
{
    static linked_list_t *list;

    if (!list)
        list = NULL;
    return &list;
}

int draw_hist(linked_list_t *node, histo_arg_t args)
{
    struct tm *time;

    for (long i = 0; i != args.nb && node; i++) {
        time = localtime(&((history_t *)(node->data))->time);
        if (!args.h)
            printf("%5li\t%i:%i\t%s\n", ((history_t *)(node->data))->id,
            time->tm_hour, time->tm_min, ((history_t *)(node->data))->cmd);
        else
            printf("%s\n", ((history_t *)(node->data))->cmd);
        node = node->next;
    }
    return 0;
}

int draw_rev_hist(int nb, linked_list_t *node, histo_arg_t args)
{
    struct tm *time;

    if (nb == 0 || !node)
        return 0;
    draw_rev_hist(nb - 1, node->next, args);
    time = localtime(&((history_t *)(node->data))->time);
    if (!args.h)
        printf("%5li\t%i:%i\t%s\n", ((history_t *)(node->data))->id,
        time->tm_hour, time->tm_min, ((history_t *)(node->data))->cmd);
    else
        printf("%s\n", ((history_t *)(node->data))->cmd);
    return 0;
}

long get_id_ind(bool init)
{
    static long id;

    if (init)
        id = -1;
    id++;
    return id;
}

void init_history(void)
{
    get_id_ind(1);
    load_history(".mysh_history");
}
