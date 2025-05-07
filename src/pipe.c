/*
** EPITECH PROJECT, 2025
** pipe.c
** File description:
** pipe.c
*/

#include "mysh.h"
#include <stdio.h>
#include <unistd.h>

int execute_fonc(char *line, char **env)
{
    exit_info()->ret_val = test_builtin(line, env);
    if (exit_info()->ret_val == -1)
        exit_info()->ret_val = execcommand(line, env);
    return exit_info()->ret_val;
}

void not_first(int prev_fd, pid_t pid)
{
    if (pid == 0){
        if (prev_fd != -1){
            dup2(prev_fd, STDIN_FILENO);
            close(prev_fd);
        }
    } else
        if (prev_fd != -1)
            close(prev_fd);
}

int not_last(pid_t pid, int *pipefd, int len_tab, int i)
{
    if (pid == 0){
        if (i < len_tab - 1){
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            return 0;
        }
    } else
        if (i < len_tab - 1){
            close(pipefd[1]);
            return pipefd[0];
        }
    return 0;
}

char exec_pipe(char **arr, int len_tab, char **env)
{
    pid_t pid = -1;
    int prev_fd = -1;
    int status = 0;
    int pipefd[2];

    for (int i = 0; arr[i]; i++){
        pipe(pipefd);
        pid = fork();
        if (pid == 0){
            not_first(prev_fd, pid);
            not_last(pid, pipefd, len_tab, i);
            status = test_redir(arr[i], env);
            exit(0);
        }
        not_first(prev_fd, pid);
        prev_fd = not_last(pid, pipefd, len_tab, i);
    }
    for (int i = 0; i < len_tab; i++)
        wait(NULL);
    return status;
}

static int count_pipe(char *line)
{
    int count = 0;

    for (int i = 0; line[i]; i++){
        if (is_skip(line[i]))
            skip_str(line, &i, i);
        if (line[i] == *PIPECHAR)
            count++;
    }
    return count;
}

char built_pipe(char *line_cmd, char **env)
{
    int len_tab = 0;
    char **cmd_split = str_skip_arr(line_cmd, "|");

    if (!cmd_split)
        return 84;
    len_tab = len_arr(cmd_split);
    if (count_pipe(line_cmd) >= len_tab){
        write(2, "Invalid null command.\n", 22);
        return 84;
    }
    if (len_tab == 1){
        free_tab(cmd_split);
        return test_redir(line_cmd, env);
    } else
        exit_info()->ret_val = exec_pipe(cmd_split, len_tab, env);
    free_tab(cmd_split);
    return exit_info()->ret_val;
}
