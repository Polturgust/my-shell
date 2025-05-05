/*
** EPITECH PROJECT, 2024
** mysh
** File description:
** mysh
*/

#include "include/mysh.h"
#include "include/list.h"
#include "include/my.h"
#include "lib/my/my.h"
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

static void test_error(int status)
{
    if (WIFSIGNALED(status)){
        write(2, "Segmentation fault ", 19);
        if (WCOREDUMP(status))
            write(2, "(core dumped)", 14);
        write(2, "\n", 1);
    }
}

static void check_err(char *arg)
{
    if (errno == ENOEXEC){
        write(2, arg, my_strlen(arg));
        write(2, ": ", 2);
        write(2, strerror(errno), my_strlen(strerror(errno)));
        write(2, ". Binary file not executable.\n", 30);
    }
}

int use_children(char **env, char **argv)
{
    int status = 0;
    int res_child = 0;
    pid_t child = fork();
    char **freearr = argv;

    if (child == 0){
        res_child = execve(argv[0], argv, env);
        if (res_child == -1)
            check_err(argv[0]);
        exit(res_child);
    }
    argv = NULL;
    free_tab(freearr);
    waitpid(child, &status, 0);
    test_error(status);
    return status;
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

int execcommand(char *argv, char **env)
{
    char **arg = my_str_to_word_array(argv, " \t");
    char *freeable = NULL;
    char *buffer = NULL;
    struct stat size;
    linked_list_t *i = *create_env(env);

    if (stat(arg[0], &size) != -1 && S_ISREG(size.st_mode))
        return use_children(env, arg);
    for (; i->next &&
    my_strcmp("PATH", ((data_t *)i->data)->var) != 0; i = i->next);
    buffer = verifpath(((data_t *)i->data)->data, arg[0]);
    if (!buffer){
        free_tab(arg);
        return -1;
    }
    freeable = arg[0];
    arg[0] = buffer;
    free(freeable);
    return use_children(env, arg) / 256;
}

static int prompt(char **line, size_t *l)
{
    char *path = getcwd(NULL, 0);

    if (isatty(STDIN_FILENO)){
        write(1, "\033[35m", 6);
        write(1, path, my_strlen(path));
        write(1, "\033[36m\uf105\n\u279c \033[00m", 19);
    }
    free(path);
    return getline(line, l, stdin);
}

int minishell(char **env)
{
    char *line = NULL;
    size_t l = 0;
    int verif = prompt(&line, &l);

    while (verif != -1 && !exit_info()->do_exit) {
        line[my_strlen(line) - 1] = '\0';
        if (!line || *line == '\0'){
            verif = prompt(&line, &l);
            continue;
        }
        add_history(line);
        exit_info()->ret_val = test_semi(line, env);
        if (exit_info()->do_exit)
            break;
        verif = prompt(&line, &l);
    }
    if ((verif == -1 || exit_info()->do_exit) && isatty(0))
        write(1, "exit\n", 5);
    free(line);
    return exit_info()->ret_val;
}
