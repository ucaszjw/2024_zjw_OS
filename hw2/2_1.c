#include <stdio.h>
#include <stdlib.h>
int a = 1;
int b;
int main(){
    int *array = malloc(sizeof(int) * 2);
    array[0] = a;
    array[1] = b;
    printf("array[0] = %d\n", array[0]);
    printf("array[1] = %d\n", array[1]);
    free(array);
    return 0;
}