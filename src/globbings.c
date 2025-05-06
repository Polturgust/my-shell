/*
** EPITECH PROJECT, 2024
** B-PSU-200-NCE-2-1-42sh-florent.serra
** File description:
** globbings.c
*/

#include "mysh.h"
#include <glob.h>

int has_globbing(const char *str)
{
    for(int i = 0; str[i]; i++)
        if (str[i] == '*' || str[i] == '?' || str[i] == '[')
        return 1;
    return 0;
}

int count_matches(glob_t *glob_result)
{
    int count = 0;

    while(glob_result->gl_pathv[count])
        count++;
    return count;
}

char **append_glob_result(glob_t *glob_result)
{
    int count = count_matches(glob_result);
    char **result = malloc(sizeof(char *) * (count + 1));

    if(!result)
        return NULL;
    for(int i = 0; i < count; i++)
        result[i] = my_strdup(glob_result->gl_pathv[i]);
    result[count] = NULL;
    return result;
}

char **do_globbings(char **argv)
{
    char **new_args = NULL;
    glob_t glob_result;
    int flags = 0;

    glob_result.gl_pathc = 0;
    glob_result.gl_pathv = NULL;
    for(int i = 0; argv[i]; i++) {
        if(has_globbing(argv[i]))
            glob(argv[i], flags, NULL, &glob_result);
        else
            glob(argv[i], flags | GLOB_NOCHECK, NULL, &glob_result);
        flags = GLOB_APPEND;
    }
    new_args = append_glob_result(&glob_result);
    globfree(&glob_result);
    free_tab(argv);
    return new_args;
}
