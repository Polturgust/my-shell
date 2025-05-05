/*
** EPITECH PROJECT, 2025
** exit.c
** File description:
** exit.c
*/

#include "list.h"
#include "mysh.h"

exit_t *exit_info(void)
{
    static exit_t *exit;

    if (!exit) {
        exit = malloc(sizeof(exit));
        if (!exit)
            return NULL;
        exit->do_exit = 0;
        exit->ret_val = 0;
    }
    return exit;
}

int my_exit(__attribute__((unused)) linked_list_t **env, char **argv)
{
    if (len_arr(argv) > 2) {
        write(2, "exit: Expression Syntax.\n", 26);
        return 1;
    }
    exit_info()->do_exit = 1;
    if (len_arr(argv) == 1)
        return exit_info()->ret_val;
    if (my_str_isnum(argv[1]))
        return atoi(argv[1]);
    exit_info()->do_exit = 0;
    write(2, "exit: Expression Syntax.\n", 26);
    return 1;
}
