/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** env2.c - environment removal and export
*/

#include <stdlib.h>
#include <string.h>
#include "shell.h"

vec_t *env_vec(void);

void env_unset(const char *key)
{
    vec_t *env = env_vec();
    size_t klen = strlen(key);
    size_t i = 0;

    for (; i < env->len; i++)
        if (strncmp(env->items[i], key, klen) == 0
            && env->items[i][klen] == '=')
            break;
    if (i == env->len)
        return;
    free(env->items[i]);
    memmove(&env->items[i], &env->items[i + 1],
        sizeof(char *) * (env->len - i));
    env->len--;
}

char **env_array(void)
{
    return env_vec()->items;
}

void env_free(void)
{
    vec_free(env_vec());
}
