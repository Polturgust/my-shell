/*
** EPITECH PROJECT, 2024
** my h
** File description:
** my h qui contient libmy
*/
#ifndef MYSH_H_
    #define MYSH_H_
    #include "my.h"
    #include <string.h>
    #include "list.h"
    #include <sys/stat.h>
    #include <sys/wait.h>
    #include <sys/signal.h>
    #include <fcntl.h>
    #define TCSHENV "NLSPATH=/usr/bin/"
    #define PIPECHAR "|"

typedef struct skip_s {
    char sep;
    char sep_fin;
    int count;
} skip_t;

typedef struct data_s {
    char *var;
    char *data;
} data_t;

int parenthesis(char *line, char **env);
char **str_skip_arr(char *str, char *slice);
int skip_str(char *line, int *count, int idc);
char is_skip(char c);
linked_list_t *get_skip(void);
int my_or(char *line, char **env);
int my_and(char *line, char **env);
int minishell(char **env);
char *arr_to_str(char **arr, char sep);
int use_children(char **env, char **argv);
int check_alias(char *arr, char **env);
linked_list_t **get_alias(void);
int aliases(linked_list_t **list, char **arr);
int karentede(linked_list_t **list, char **argv);
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
#endif /* MYSH_H_ */
