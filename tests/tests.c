/*
** EPITECH PROJECT, 2024
** B-PSU-200-NCE-2-1-42sh-florent.serra
** File description:
** test.c
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../include/mysh.h"

Test(my_str_isalpha, alpa_test)
{
    cr_expect(my_str_isalpha("str") == 1, "str is not alpanum ?");
    cr_expect(my_str_isalpha("#str") == 0, "#str is alphanum ?");
}
