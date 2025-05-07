/*
** EPITECH PROJECT, 2025
** parentheses
** File description:
** parentheses
*/

#include "mysh.h"

int count_parenth(char *str)
{
    int count = 1;
    int i = 1;

    for (; str[i] && count != 0; i++){
        if (str[i] == '(')
            count++;
        if (str[i] == ')')
            count--;
    }
    if (count > 0)
        return -1;
    return i;
}

int execute_parenth(char **line, char **env)
{
    pid_t child = fork();
    int status;

    if (child == 0){
        test_semi(*line, env);
        exit(0);
    }
    wait(&status);
    free(*line);
    return status;
}

int parenthesis(char *line, char **env)
{
    char **str = str_skip_arr(line, " ");
    char *new_line = NULL;
    int retval = 0;
    int count = 0;

    if (!is_in('(', line)){
        free_tab(str);
        return execute_fonc(line, env);
    }
    if (len_arr(str) > 1 || (*str)[0] != '('){
        dprintf(STDIN_FILENO, "Badly placed ()'s\n");
        free_tab(str);
        return 84;
    }
    count = count_parenth(*str);
    if (count == -1)
        return 84;
    new_line = strndup(line + 1, count - 2);
    retval = execute_parenth(&new_line, env);
    return retval;
}
