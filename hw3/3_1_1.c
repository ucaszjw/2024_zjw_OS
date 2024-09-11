#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int array[1000000];
    for (int i = 0; i < 1000000; i++)
        array[i] = i+1;
    long sum = 0;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < 1000000; i++)
        sum += array[i];
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Sum: %ld\n", sum);
    printf("Time: %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec));
    return 0;
}