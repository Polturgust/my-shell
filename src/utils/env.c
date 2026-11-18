/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** env.c - the environment passed to every program, kept as KEY=VALUE
*/

#include <stdlib.h>
#include <string.h>
#include "shell.h"

vec_t *env_vec(void)
{
    static vec_t env = {NULL, 0, 0};

    if (!env.items)
        vec_init(&env);
    return &env;
}

static long env_index(const char *key)
{
    vec_t *env = env_vec();
    size_t klen = strlen(key);

    for (size_t i = 0; i < env->len; i++)
        if (strncmp(env->items[i], key, klen) == 0
            && env->items[i][klen] == '=')
            return (long)i;
    return -1;
}

void env_init(char **environ_src)
{
    vec_t *env = env_vec();

    for (size_t i = 0; environ_src && environ_src[i]; i++)
        if (strchr(environ_src[i], '='))
            vec_push(env, xstrdup(environ_src[i]));
    if (!env_get("PATH"))
        env_set("PATH", DEFAULT_PATH);
}

const char *env_get(const char *key)
{
    long idx = env_index(key);

    if (idx < 0)
        return NULL;
    return strchr(env_vec()->items[idx], '=') + 1;
}

void env_set(const char *key, const char *value)
{
    vec_t *env = env_vec();
    long idx = env_index(key);
    size_t len = strlen(key) + strlen(value ? value : "") + 2;
    char *entry = xmalloc(len);

    strcpy(entry, key);
    strcat(entry, "=");
    strcat(entry, value ? value : "");
    if (idx < 0) {
        vec_push(env, entry);
        return;
    }
    free(env->items[idx]);
    env->items[idx] = entry;
}
