/*
** EPITECH PROJECT, 2024
** sort int
** File description:
** in array
*/
void loop(int *array, int i)
{
    int c;

    for (int j = 0; j < i - 1; j++){
            if (array[j] > array[j + 1]){
                c = array[j];
                array[j] = array[j + 1];
                array[j + 1] = c;
            }
    }
}

void my_sort_int_array(int *array, int size)
{
    for (int i = 0; i < size - 1; i++){
        loop(array, size);
    }
}
