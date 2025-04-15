/*
** EPITECH PROJECT, 2024
** cpy str
** File description:
** bah ça copie
*/

char *my_strcpy(char *dest, const char *src)
{
    int i = 0;

    while (src[i]){
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}
