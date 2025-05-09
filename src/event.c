/*
** EPITECH PROJECT, 2024
** B-PSU-200-NCE-2-1-42sh-florent.serra
** File description:
** event.c
*/

#include "../include/mysh.h"

static char *find_event_by_id(long id)
{
    linked_list_t *list = *hist_info();

    while (list)
        if (((history_t *)list->data)->id == id)
            return ((history_t *)list->data)->cmd;
    dprintf(2, "%li: Event not found.\n", id);
    exit_info()->ret_val = 1;
    return NULL;
}

static char *find_event_by_name(char *cmd)
{
    linked_list_t *list = *hist_info();

    while (list)
        if (strncmp(((history_t *)list->data)->cmd, cmd, strlen(cmd)) == 0)
            return ((history_t *)list->data)->cmd;
    dprintf(2, "%s: Event not found.\n", cmd);
    exit_info()->ret_val = 1;
    return NULL;
}

char *get_event(char *event)
{
    if (!event || event[0] != '!') {
        return NULL;
        exit_info()->ret_val = 1;
    }
    if (my_str_isnum(&event[1]))
        return find_event_by_id(atol(&event[1]));
    return find_event_by_name(&event[1]);
}
