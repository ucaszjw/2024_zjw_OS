#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#define MAX 10000000
#define max(a, b) ((a) > (b) ? (a) : (b))

int idx = 0;
int idx_even = 0;
int idx_odd = 0;
int data[MAX] = {0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *even(void *arg) {
    while (idx < MAX){
        pthread_mutex_lock(&mutex);
        int count = 0;
        int i = idx_even;
        while (count < 200 && i < MAX){
            data[idx] = i;
            idx++;
            i+=2;
            count++;
        }
        idx_even += 400;
        pthread_mutex_unlock(&mutex); 
    }
    return NULL;
}

void *odd(void *arg) {
    while (idx < MAX){
        pthread_mutex_lock(&mutex);
        int count = 0;
        int i = idx_odd;
        while (count < 200 && i < MAX){
            data[idx] = i+1;
            idx++;
            i+=2;
            count++;
        }
        idx_odd += 400;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    struct timespec start, end;
    pthread_mutex_init(&mutex, NULL);
    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_create(&t1, NULL, even, NULL);
    pthread_create(&t2, NULL, odd, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    clock_gettime(CLOCK_MONOTONIC, &end);
    pthread_mutex_destroy(&mutex);

    int max_diff = 0;
    long sum = 0;
    for (int i = 0; i+1 < MAX; i++){
        max_diff = max(max_diff, abs(data[i] - data[i+1]));
    }
    for (int i = 0; i < MAX; i++)
        sum += data[i];    
    
    printf("Max difference between two consecutive elements: %d\n", max_diff);
    printf("Sum of all elements: %ld\n", sum);
    printf("Time taken: %lf ns\n", (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec));
    return 0;
}
    