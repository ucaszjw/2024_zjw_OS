#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 8
#define NUM_INTS 16

int final_sum = 0;

typedef struct {
    int data[NUM_INTS];
    int index;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Monitor;

Monitor monitor;

void* thread_func(void* arg) {
    int id = *(int*)arg;

    // 初始化数据
    int a = rand() % 100;
    int b = rand() % 100;
    monitor.data[monitor.index++] = a;
    monitor.data[monitor.index++] = b;

    // 模拟不均衡性
    usleep((rand() % 10 + 1) * 1000);

    int sum = a + b;
    printf("Thread %d: %d + %d = %d\n", id, a, b, sum);

    pthread_mutex_lock(&monitor.mutex);
    if (monitor.count > NUM_INTS)
        pthread_cond_wait(&monitor.cond, &monitor.mutex);
    final_sum += sum;
    monitor.count++;
    pthread_mutex_unlock(&monitor.mutex);
    return NULL;
}

int main() {
    srand(time(NULL));

    monitor.index = 0;
    monitor.count = 0;
    pthread_mutex_init(&monitor.mutex, NULL);
    pthread_cond_init(&monitor.cond, NULL);

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // 创建线程
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]);
    }

    // 等待所有线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 打印结果
    printf("Final sum: %d\n", final_sum);

    // 销毁互斥锁和条件变量
    pthread_mutex_destroy(&monitor.mutex);
    pthread_cond_destroy(&monitor.cond);

    return 0;
}