#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#define THREADS 4
int array[1000000];
int sum[THREADS] = {0};

typedef struct {
    int *array;
    int id;
} thread_data_t;

void* thread_sum(void* arg){
    thread_data_t *data = (thread_data_t*)arg;
    int *array = data->array;
    int id = data->id;
    int local_sum = 0;
    int start = id * 1000000 / THREADS;
    int end = (id + 1) * 1000000 / THREADS;
    for (int i = start; i < end; i++)
        local_sum += array[i];
    sum[id] = local_sum;
    return NULL;
}

void* worker(void* arg){
    thread_data_t *data = (thread_data_t*)arg;
    int core_id = data->id;
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    sched_setaffinity(0, sizeof(cpuset), &cpuset);
    thread_sum(arg);
    return NULL;
}

int main(){    
    for (int i = 0; i < 1000000; i++)
        array[i] = i;

    struct timespec start, end;
    pthread_t threads[THREADS];
    thread_data_t thread_data[THREADS];
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < THREADS; i++){
        thread_data[i].array = array;
        thread_data[i].id = i;
        pthread_create(&threads[i], NULL, worker, &thread_data[i]);
    }
    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    int total_sum = 0;
    for (int i = 0; i < THREADS; i++)
        total_sum += sum[i];
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Sum: %d\n", total_sum);
    printf("Time: %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec));
    return 0;
}