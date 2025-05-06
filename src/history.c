/*
** EPITECH PROJECT, 2024
** B-PSU-200-NCE-2-1-42sh-florent.serra
** File description:
** history.c
*/

#include "../include/mysh.h"

static linked_list_t **hist_info(void)
{
    static linked_list_t *list;

    if (!list)
        list = NULL;
    return &list;
}

static int draw_rev_hist(int nb, linked_list_t *node, int size)
{
    struct tm *time;

    if (nb == 0 || !node)
        return 0;
    draw_rev_hist(nb - 1, node->next, size);
    time = localtime(&((history_t *)(node->data))->time);
    printf("%5li\t%i:%i\t%s\n", ((history_t *)(node->data))->id,
    time->tm_hour, time->tm_min, ((history_t *)(node->data))->cmd);
    return 0;
}

int history(__attribute__((unused)) linked_list_t **env, char **arg)
{
    int argc = len_arr(arg);

    if (argc == 1)
        return draw_rev_hist(-1, *hist_info(), get_len(*hist_info()));
    if (argc == 2 && arg[1][0] != '-') {
        if (my_str_isnum(arg[1]))
            return draw_rev_hist((atoi(arg[1]) == 0 ? 1 : atoi(arg[1]))
            , *hist_info(), get_len(*hist_info()));
        else {
            write(2, "history: Badly formed number.\n", 31);
            return 1;
        }
    }
    return 0;
}

static unsigned long get_id_ind(void)
{
    linked_list_t *l = *hist_info();

    if (!l)
        return 1;
    return ((history_t *)l->data)->id + 1;
}

void add_history(char *cmd)
{
    history_t *n_hist = malloc(sizeof(history_t));

    if (!n_hist)
        return;
    n_hist->id = get_id_ind();
    n_hist->time = time(NULL);
    n_hist->cmd = strdup(cmd);
    push_front(hist_info(), (void *)n_hist);
}

void set_history(char *path)
{
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    linked_list_t **list = hist_info();
    history_t *el;

    while (*list) {
        el = pop_front(list);
        dprintf(fd, "%s;%li;%li\n", el->cmd, el->id, el->time);
        free(el->cmd);
        free(el);
        el = NULL;
    }
}
