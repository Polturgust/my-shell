/*
** EPITECH PROJECT, 2024
** B-PSU-200-NCE-2-1-42sh-florent.serra
** File description:
** history.c
*/

#include "../include/mysh.h"

static int bad_arg_history(int argc, char **argv)
{
    if (argc > 3)
        return my_puterror_str("history: Too many arguments.\n");
    if (argc == 1)
        return 0;
    if ((argv[1][0] != '-' && !my_str_isnum(argv[1])) || (argc == 3 &&
        (argv[2][0] != '-' && !my_str_isnum(argv[2]))))
        return my_puterror_str("history: Badly formed number.\n");
    if ((argv[1][0] == '-' && strspn(&argv[1][1], "chrSLMT") !=
    strlen(&argv[1][1])) || (argc == 3 && (argv[2][0] == '-' && strspn(
    &argv[2][1], "chrSLMT") != strlen(&argv[2][1]))))
        return my_puterror_str("Usage: history [-chrSLMT] [# number of"
        " events].\n");
    return 0;
}

static int do_arg(char *arg, histo_arg_t *h_arg)
{
    if (arg[0] != '-' && h_arg->nb == -1) {
        h_arg->nb = (atol(arg) == 0 ? 1 : atol(arg));
        return 1;
    }
    for (int i = 1; arg[i]; i++) {
        h_arg->h = (arg[i] == 'h' ? 1 : 0);
        h_arg->r = (arg[i] == 'r' ? 1 : 0);
        if (arg[i] == 'c')
            remove_history();
        if (arg[i] == 'S')
            set_history(".mysh_history");
        if (arg[i] == 'L')
            load_history(".mysh_history");
        if (arg[i] == 'M')
            continue;
        if (arg[i] == 'T')
            continue;
    }
    return 0;
}

int history(__attribute__((unused)) linked_list_t **env, char **arg)
{
    histo_arg_t h_args;
    bool print = 0;

    h_args.h = 0;
    h_args.nb = -1;
    h_args.r = 0;
    if (bad_arg_history(len_arr(arg), arg))
        return 1;
    if (len_arr(arg) == 1)
        return draw_rev_hist(-1, *hist_info(), h_args);
    for (int i = 1; arg[i]; i++)
        print = do_arg(arg[i], &h_args);
    if (print || h_args.h || h_args.r) {
        if (h_args.r)
            draw_hist(*hist_info(), h_args);
        else
            draw_rev_hist(h_args.nb, *hist_info(), h_args);
    }
    return 0;
}

void add_history(char *cmd)
{
    history_t *n_hist = malloc(sizeof(history_t));

    if (!n_hist)
        return;
    n_hist->id = get_id_ind(0);
    n_hist->time = time(NULL);
    n_hist->cmd = strdup(cmd);
    push_front(hist_info(), (void *)n_hist);
}

void set_history(char *path)
{
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    linked_list_t *list = *hist_info();
    history_t *el;

    while (list) {
        el = (history_t *)(list->data);
        dprintf(fd, "%s;%li;%li\n", el->cmd, el->id, el->time);
        list = list->next;
    }
}

void load_history(char *path)
{
    char *file = get_file(path);
    char **arr;
    history_t *n_hist = NULL;
    char *end;

    if (!file)
        return;
    arr = my_str_to_word_array(file, "\n;");
    for (int i = 0; arr[i]; i += 3) {
        n_hist = malloc(sizeof(history_t));
        n_hist->id = get_id_ind(0);
        n_hist->time = strtoul(arr[i + 2], &end, 10);
        n_hist->cmd = strdup(arr[i]);
        push_front(hist_info(), (void *)n_hist);
    }
}

void remove_history(void)
{
    linked_list_t **list = hist_info();
    history_t *el;

    while (*list) {
        el = pop_front(list);
        free(el->cmd);
        free(el);
        el = NULL;
    }
    *list = NULL;
}
