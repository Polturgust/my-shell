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
    #include "variables.h"
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

typedef struct histo_arg_s {
    bool h;
    bool r;
    long nb;
}histo_arg_t;

typedef struct history_s {
    char *cmd;
    long id;
    time_t time;
} history_t;

typedef struct exit_s {
    bool do_exit;
    int ret_val;
} exit_t;

exit_t *exit_info(void);

typedef struct skip_s {
    char sep;
    char sep_fin;
    int count;
} skip_t;

typedef struct data_s {
    char *var;
    char *data;
} data_t;

int backticks(char *line, char **env);
char rd_droite(char *argv, char **env);
int parenthesis(char *line, char **env);
char **str_skip_arr(char *str, char *slice);
int skip_str(char *line, int *count, int idc);
char is_skip(char c);
linked_list_t *get_skip(void);
int my_or(char *line, char **env);
int my_and(char *line, char **env);
int minishell(char **env);
int execcommand(char *argv, char **env);
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
linked_list_t **hist_info(void);
int draw_rev_hist(int nb, linked_list_t *node, histo_arg_t args);
int draw_hist(linked_list_t *node, histo_arg_t args);
long get_id_ind(bool init);
void load_history(char *path);
void init_history(void);
void add_history(char *cmd);
void set_history(char *path);
void remove_history(void);

//globbings
char **do_globbings(char **argv);


//inhibitors
char **remove_quotes(char **args);

// event
///////////////////
//
// get_event take a str with this format: '!n' or '!cmd'
// in case of cmd is found or if an event occured at the n-th indice, get_event
// return the command. Else get_event return NULL and exit_into()->ret_val is
// set at 1.
//
///////////////////
char *get_event(char *event);

#endif /* MYSH_H_ */
