/*
** EPITECH PROJECT, 2025
** env.c
** File description:
** env.c
*/

#include "list.h"
#include "my.h"
#include "mysh.h"
#include <sys/stat.h>
#include <unistd.h>

int test_args(char **arg, int retval, char *pwd)
{
    struct stat st;
    int file = 0;
    char *str = my_strconcat(pwd, arg[1]);

    file = stat(str, &st);
    if (file != -1 && !S_ISREG(st.st_mode))
        return 0;
    if (retval < -1){
        write(2, arg[1], my_strlen(arg[1]));
        if (file == -1){
            write(2, ": No such file or directory.\n", 29);
            return 1;
        }
        if (S_ISREG(st.st_mode)){
            write(2, ": Not a directory.\n", 19);
            return 1;
        }
    }
    free(str);
    return retval;
}

char *create_path(char *arg)
{
    char *pwd = getcwd(NULL, 0);
    char *pathcomp = NULL;

    if (!arg || !pwd)
        return NULL;
    pathcomp = my_strconcat(pwd, "/");
    free(pwd);
    pwd = pathcomp;
    pathcomp = my_strconcat(pwd, arg);
    free(pwd);
    return pathcomp;
}

int test_path(linked_list_t **env, char **path, char **arg)
{
    int retval = 1;
    char *pwd = getcwd(NULL, 0);

    for (linked_list_t *i = *env; i; i = i->next)
        if (my_strcmp(((data_t *)i->data)->var, "HOME") == 0)
            *path = my_strdup(((data_t *)i->data)->data);
    if (!*path)
        return 84;
    if (!arg[1] || my_strcmp(arg[1], "~") == 0){
        etenv(env, (char *[4]){NULL, "OLDPWD", pwd, NULL});
        free(pwd);
        retval = chdir(*path);
        pwd = getcwd(NULL, 0);
        etenv(env, (char *[4]){NULL, "PWD", pwd, NULL});
    }
    free(pwd);
    free(*path);
    return retval;
}

int prev_cd(linked_list_t **env)
{
    for (linked_list_t *i = *env; i; i = i->next)
        if (my_strcmp(((data_t *)i->data)->var, "OLDPWD") == 0)
            return chdir(((data_t *)i->data)->data);
    return 84;
}

static int change_env(linked_list_t **env, char *arg)
{
    int retval = 0;
    char *pwd = getcwd(NULL, 0);
    char *path = NULL;
    struct stat size;

    etenv(env, (char *[4]){NULL, "OLDPWD", pwd, NULL});
    free(pwd);
    if (stat(arg, &size) == -1){
        path = create_path(arg);
        retval = chdir(path) - 254;
        free(path);
    } else {
        retval = chdir(arg) - 254;
    }
    pwd = getcwd(NULL, 0);
    etenv(env, (char *[4]){NULL, "PWD", pwd, NULL});
    free(pwd);
    return retval;
}

int set_cd(linked_list_t **env, char **arg)
{
    char *path = NULL;
    int retval = 1;
    char *cwd = getcwd(NULL, 0);
    char *pwd = my_strconcat(cwd, "/");

    retval = test_path(env, &path, arg);
    if (retval == 84)
        return 1;
    if (retval == 0)
        return 0;
    if (my_strcmp(arg[1], "-") == 0)
        return prev_cd(env);
    retval = change_env(env, arg[1]);
    retval = test_args(arg, retval, pwd);
    free(cwd);
    free(pwd);
    return retval;
}
