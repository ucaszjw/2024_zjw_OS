#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdatomic.h>

#define MAX 10000000
#define max(a, b) ((a) > (b) ? (a) : (b))

atomic_long idx = 0;
atomic_long idx_even = 0;
atomic_long idx_odd = 0;
long data[MAX] = {0};

void *even(void *arg) {
    while (atomic_load(&idx) < MAX) {
        int count = 0;
        long i = atomic_load(&idx_even);
        while (count < 200 && i < MAX) {
            data[atomic_fetch_add(&idx, 1)] = i;
            i += 2;
            count++;
        }
        atomic_fetch_add(&idx_even, 400);
    }
    return NULL;
}

void *odd(void *arg) {
    while (atomic_load(&idx) < MAX) {
        int count = 0;
        long i = atomic_load(&idx_odd);
        while (count < 200 && i < MAX) {
            data[atomic_fetch_add(&idx, 1)] = i + 1;
            i += 2;
            count++;
        }
        atomic_fetch_add(&idx_odd, 400);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_create(&t1, NULL, even, NULL);
    pthread_create(&t2, NULL, odd, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    clock_gettime(CLOCK_MONOTONIC, &end);

    long max_diff = 0;
    long sum = 0;
    for (int i = 0; i + 1 < MAX; i++) {
        max_diff = max(max_diff, labs(data[i] - data[i + 1]));
    }
    for (int i = 0; i < MAX; i++)
        sum += data[i];

    printf("Max difference between two consecutive elements: %ld\n", max_diff);
    printf("Sum of all elements: %ld\n", sum);
    printf("Time taken: %lf ns\n", (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec));
    return 0;
}