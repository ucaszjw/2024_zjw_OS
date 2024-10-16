#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h> // For O_CREAT
#include <sys/stat.h> // For mode constants

// 全局变量
int x, y, z = 2;
sem_t *S1, *S2;

void* T1(void* arg) {
    y = 1;
    y = y + 2;
    sem_post(S1); // V(S1)
    z = y + 1;
    sem_wait(S2); // P(S2)
    y = z + y;
    return NULL;
}

void* T2(void* arg) {
    x = 1;
    x = x + 1;
    sem_wait(S1); // P(S1)
    x = x + y;
    sem_post(S2); // V(S2)
    z = x + z;
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // 初始化信号量
    S1 = sem_open("/S1", O_CREAT, 0644, 0);
    S2 = sem_open("/S2", O_CREAT, 0644, 0);

    // 创建线程
    pthread_create(&thread1, NULL, T1, NULL);
    pthread_create(&thread2, NULL, T2, NULL);

    // 等待线程结束
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // 打印结果
    printf("x = %d, y = %d, z = %d\n", x, y, z);

    // 关闭信号量
    sem_close(S1);
    sem_close(S2);

    // 删除信号量
    sem_unlink("/S1");
    sem_unlink("/S2");

    return 0;
}