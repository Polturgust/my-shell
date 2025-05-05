/*
** EPITECH PROJECT, 2024
** B-CPE-200-NCE-2-1-robotfactory-charles.corsiero
** File description:
** file.c
*/

#include "my.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int is_file(char *path)
{
    DIR *dir = opendir(path);
    struct stat buffer;

    if (stat(path, &buffer) != - 1 && !dir)
        return 1;
    if (dir)
        closedir(dir);
    return 0;
}

char *get_file(char *path)
{
    char *res;
    struct stat st;
    int fd;

    if (!is_file(path))
        return NULL;
    stat(path, &st);
    fd = open(path, O_RDONLY);
    res = malloc(sizeof(char) * (st.st_size + 1));
    read(fd, res, st.st_size);
    res[st.st_size] = '\0';
    close(fd);
    return res;
}
