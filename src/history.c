/*
** EPITECH PROJECT, 2024
** B-PSU-200-NCE-2-1-42sh-florent.serra
** File description:
** history.c
*/

#include "../include/mysh.h"

static void destroy_arr(void **arr)
{
    if (!arr)
        return;
    for (int i = 0; arr[i]; i++)
        free(arr[i]);
    free(arr);
}

static linked_list_t **history_info(void)
{
    static linked_list_t *cmd;

    if (!cmd) {
        cmd = NULL;
    }
    return &cmd;
}

void add_history(char *cmd)
{
    push_front(history_info(), (void *)my_strdup(cmd));
}

static void writer(char *cmd)
{
    int fd = open(".mysh_history", O_APPEND | O_WRONLY);

    if (cmd) {
        write(fd, cmd, strlen(cmd));
        write(fd, "\n", 1);
        free(cmd);
    }
    close(fd);
}

void set_history(void)
{
    linked_list_t **history = history_info();
    int fd = open(".mysh_history", O_TRUNC | O_WRONLY | O_CREAT, 0644);

    close(fd);
    for (; *history;)
        writer(pop_front(history));
}

void get_history(void)
{
    char *file = get_file(".mysh_history");
    char **arr;

    if (!file)
        return;
    arr = my_str_to_word_array(file, "\n");
    free(file);
    for (int i = 0; arr[i] && i < 255; i++)
        add_history(arr[i]);
    destroy_arr((void **)arr);
}
