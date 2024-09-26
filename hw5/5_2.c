#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define LEN 5
#define CYCLE 5

int data[LEN] = {0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *randomize(void *arg) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    srand(ts.tv_nsec);

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < LEN; i++)
        data[i] = rand() % 20 + 1;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *printArray(void *arg) {
    int sum = 0;

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < LEN; i++){
        printf("%d ", data[i]);
        sum += data[i];
    }
    pthread_mutex_unlock(&mutex);
    printf("-> Sum: %d\n", sum);
    return NULL;
}

int main(){
    pthread_t t1, t2;
    for (int i = 0; i < CYCLE; i++){
        pthread_create(&t1, NULL, randomize, NULL);
        pthread_join(t1, NULL);
        pthread_create(&t2, NULL, printArray, NULL);
        pthread_join(t2, NULL);
    }
    return 0;
}