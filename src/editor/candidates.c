/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** candidates.c - what Tab can complete: commands, files, folders, $vars
*/

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "editor.h"

static void add_if_prefix(vec_t *out, const char *name, const char *prefix)
{
    if (strncmp(name, prefix, strlen(prefix)) == 0)
        vec_push(out, xstrdup(name));
}

void complete_commands(const char *prefix, vec_t *out)
{
    const char **names = builtin_names();
    vec_t *aliases = alias_names();
    char *dirs = xstrdup(env_get("PATH") ? env_get("PATH") : DEFAULT_PATH);
    char *save = NULL;
    DIR *dir = NULL;
    struct dirent *ent = NULL;

    for (size_t i = 0; names[i]; i++)
        add_if_prefix(out, names[i], prefix);
    for (size_t i = 0; i < aliases->len; i++)
        add_if_prefix(out, aliases->items[i], prefix);
    for (char *d = strtok_r(dirs, ":", &save); d;
        d = strtok_r(NULL, ":", &save)) {
        dir = opendir(d);
        while (dir && (ent = readdir(dir)))
            if (ent->d_name[0] != '.')
                add_if_prefix(out, ent->d_name, prefix);
        if (dir)
            closedir(dir);
    }
    free(dirs);
}

static void add_variable(vec_t *out, const char *name, size_t nlen,
    const char *prefix)
{
    char *full = xmalloc(nlen + 2);

    full[0] = '$';
    memcpy(full + 1, name, nlen);
    full[nlen + 1] = '\0';
    if (strncmp(full, prefix, strlen(prefix)) == 0)
        vec_push(out, full);
    else
        free(full);
}

void complete_variables(const char *prefix, vec_t *out)
{
    char **env = env_array();

    for (var_t *v = sh()->vars; v; v = v->next)
        add_variable(out, v->key, strlen(v->key), prefix);
    for (size_t i = 0; env[i]; i++)
        add_variable(out, env[i], strcspn(env[i], "="), prefix);
}
