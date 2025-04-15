/*
** EPITECH PROJECT, 2024
** copy n
** File description:
** tj
*/

char *my_strncpy(char *dest, char const *src, int n)
{
    int i = 0;

    while (i < n && src[i]){
        dest[i] = src[i];
        i++;
    }
    return dest;
}
