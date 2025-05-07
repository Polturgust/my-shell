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
#include "include/variables.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

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

static void shell_prompt_cycle(var_t *vars, char **line, size_t *l, int *verif)
{
    run_precmd(vars);
    *verif = prompt(line, l);
}

static void process_command_line(char *line, char **env, var_t **vars)
{
    if (!handle_variable_builtins(line, vars))
        exit_info()->ret_val = backticks(line, env);
}

void handle_input_line(char *line, char **env, var_t **vars)
{
    if (line && *line != '\0') {
        line[my_strlen(line) - 1] = '\0';
        add_history(line);
        process_command_line(line, env, vars);
    }
}

static int handle_eof_condition(int verif, var_t *vars, char **line, size_t *l)
{
    if (verif == -1 && !handle_eof(vars)) {
        shell_prompt_cycle(vars, line, l, &verif);
        return 1;
    }
    return 0;
}

int minishell(char **env)
{
    char *line = NULL;
    size_t l = 0;
    int verif;
    var_t *variables = NULL;

    init_special_variables(&variables);
    shell_prompt_cycle(variables, &line, &l, &verif);
    while (verif != -1 && !exit_info()->do_exit) {
        if (handle_eof_condition(verif, variables, &line, &l))
            continue;
        handle_input_line(line, env, &variables);
        if (exit_info()->do_exit)
            break;
        shell_prompt_cycle(variables, &line, &l, &verif);
    }
    if ((verif == -1 || exit_info()->do_exit) && isatty(0))
        write(1, "exit\n", 5);
    free(line);
    free_variables(variables);
    return exit_info()->ret_val;
}
