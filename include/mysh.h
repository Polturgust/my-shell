/*
** EPITECH PROJECT, 2024
** my h
** File description:
** my h qui contient libmy
*/
#ifndef MYSH_H_
    #define MYSH_H_
    #include "my.h"
    #include "list.h"
    #include <sys/stat.h>
    #include <sys/wait.h>
    #include <sys/signal.h>
    #include <fcntl.h>
    #include <string.h>
    #include <time.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #define TCSHENV "NLSPATH=/usr/bin/"
    #define PIPECHAR "|"

typedef struct history_s {
    char *cmd;
    unsigned long id;
    time_t time;
} history_t;

typedef struct exit_s {
    bool do_exit;
    int ret_val;
} exit_t;
exit_t *exit_info(void);

typedef struct data_s {
    char *var;
    char *data;
} data_t;

int minishell(char **env);
char *arr_to_str(char **arr, char sep);
int use_children(char **env, char **argv);
int check_alias(char *arr, char **env);
linked_list_t **get_alias(void);
int aliases(linked_list_t **list, char **arr);
int my_exit(linked_list_t **list, char **argv);
int history(linked_list_t **env, char **arg);
char *create_path(char *arg);
char test_redir(char *argv, char **env);
char test_semi(char *line, char **env);
char built_pipe(char *line_cmd, char **env);
int execute_fonc(char *line, char **env);
void handler(int sig);
char built_pipe(char *line_cmd, char **env);
int execcommand(char *argv, char **env);
int set_cd(linked_list_t **, char **);
int len_arr(char **);
int nsetenv(linked_list_t **, char **);
int test_builtin(char *, char **env);
int etenv(linked_list_t **, char **);
int penv(linked_list_t **, char **);
void destroy_env(linked_list_t **);
linked_list_t **create_env(char **);

//history
void add_history(char *cmd);
void set_history(char *path);
#endif /* MYSH_H_ */
