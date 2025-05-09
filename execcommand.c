/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** execcommand.c
*/
#include "include/mysh.h"
#include "include/list.h"
#include "include/my.h"
#include "lib/my/my.h"
#include "include/variables.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static int errorcomm(char *buffer, struct stat *size, char *arg)
{
    if (stat(arg, size) != -1 && S_ISDIR(size->st_mode)){
        write(2, arg, my_strlen(arg));
        write(2, ": Permission denied.\n", 21);
        return -1;
    }
    if (stat(buffer, size) == -1){
        free(buffer);
        write(2, arg, my_strlen(arg));
        write(2, ": Command not found.\n", 21);
        return -1;
    }
    return 0;
}

static char *verifpath(char *exec, char *arg)
{
    char *buffer = NULL;
    struct
    stat size;
    char *freeable = NULL;
    char **arr = my_str_to_word_array(exec, ":");

    for (int j = 0; arr[j] && (stat(buffer, &size) == -1
    || !S_ISREG(size.st_mode)); j++){
        freeable = buffer;
        buffer = my_strconcat(arr[j], "/");
        free(freeable);
        freeable = buffer;
        buffer = my_strconcat(buffer, arg);
        free(freeable);
    }
    free_tab(arr);
    if (errorcomm(buffer, &size, arg) == -1)
        return NULL;
    return buffer;
}

static char **modify_arg(char **arg)
{
    if (!arg)
        return NULL;
    arg = remove_quotes(arg);
    arg = do_globbings(arg);
    return arg;
}

int execcommand(char *argv, char **env)
{
    char **arg = my_str_to_word_array(argv, " \t");
    char *buffer = NULL;
    struct stat size;
    linked_list_t *i = *create_env(env);

    arg = modify_arg(arg);
    if (!arg)
        return -1;
    if (stat(arg[0], &size) != -1 && S_ISREG(size.st_mode))
        return use_children(env, arg);
    for (; i->next &&
    my_strcmp("PATH", ((data_t *)i->data)->var) != 0; i = i->next);
    buffer = verifpath(((data_t *)i->data)->data, arg[0]);
    if (!buffer){
        free_tab(arg);
        return -1;
    }
    free(arg[0]);
    arg[0] = buffer;
    return use_children(env, arg) / 256;
}
