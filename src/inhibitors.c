/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** inhibitors.c
*/

#include "../include/mysh.h"
#include "../include/list.h"
#include "../include/my.h"
#include "../lib/my/my.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static char *remove_enclosing_quotes(char *str)
{
    int len = my_strlen(str);
    char *new = NULL;

    if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\''))) {
        new = malloc(len - 1);
        if (!new)
            return NULL;
        my_strncpy(new, str + 1, len - 2);
        new[len - 2] = '\0';
        return new;
    }
    return my_strdup(str);
}
