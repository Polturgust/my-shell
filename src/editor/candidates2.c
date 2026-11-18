/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** candidates2.c - file and folder names
*/

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "editor.h"


static char *lookup_dir(const char *dir_part)
{
    size_t consumed = 0;
    char *home = NULL;
    char *full = NULL;

    if (!dir_part[0])
        return xstrdup(".");
    if (dir_part[0] != '~')
        return xstrdup(dir_part);
    home = expand_tilde(dir_part, &consumed);
    if (!home)
        return xstrdup(dir_part);
    full = xmalloc(strlen(home) + strlen(dir_part + consumed) + 1);
    strcpy(full, home);
    strcat(full, dir_part + consumed);
    free(home);
    return full;
}

static void add_file(vec_t *out, const char *dir, const char *dir_part,
    const char *name, bool command)
{
    char *path = path_join(dir, name);
    struct stat st;
    bool is_dir = stat(path, &st) == 0 && S_ISDIR(st.st_mode);
    char *text = NULL;

    if (!command || is_dir || access(path, X_OK) == 0) {
        text = xmalloc(strlen(dir_part) + strlen(name) + 2);
        strcpy(text, dir_part);
        strcat(text, name);
        if (is_dir)
            strcat(text, "/");
        vec_push(out, text);
    }
    free(path);
}

static void add_files(const char *word, bool command, vec_t *out)
{
    const char *slash = strrchr(word, '/');
    char *dir_part = xstrndup(word, slash ? (size_t)(slash - word + 1) : 0);
    const char *base = slash ? slash + 1 : word;
    char *dir_path = lookup_dir(dir_part);
    DIR *dir = opendir(dir_path);
    struct dirent *ent = NULL;

    while (dir && (ent = readdir(dir))) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;
        if (ent->d_name[0] == '.' && base[0] != '.')
            continue;
        if (strncmp(ent->d_name, base, strlen(base)) == 0)
            add_file(out, dir_path, dir_part, ent->d_name, command);
    }
    if (dir)
        closedir(dir);
    free(dir_part);
    free(dir_path);
}

static void unique(vec_t *v)
{
    size_t keep = 0;

    vec_sort(v);
    for (size_t i = 0; i < v->len; i++) {
        if (keep && strcmp(v->items[keep - 1], v->items[i]) == 0) {
            free(v->items[i]);
            continue;
        }
        v->items[keep++] = v->items[i];
    }
    v->len = keep;
    v->items[keep] = NULL;
}

void complete_candidates(const char *word, bool command, vec_t *out)
{
    if (word[0] == '$')
        complete_variables(word, out);
    else if (command && !strchr(word, '/'))
        complete_commands(word, out);
    else
        add_files(word, command, out);
    unique(out);
}
