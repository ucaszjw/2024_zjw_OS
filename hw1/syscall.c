#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <syscall.h>

void gettime_glibc() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    getpid();
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("getpid costs %ld ns through library function provided by glibc\n", (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec));
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    open("test.txt", O_CREAT);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("open costs %ld ns through library function provided by glibc\n", (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec));
}

void gettime_syscall() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    syscall(SYS_getpid);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("getpid costs %ld ns through syscall function directly\n", (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec));
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    syscall(SYS_open, "test.txt", O_CREAT);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("open costs %ld ns through syscall function directly\n", (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec));
}

void gettime_asm(){
    struct timespec start, end;
    int pid;
    clock_gettime(CLOCK_MONOTONIC, &start);
    asm volatile(
        "mov $39, %%rax\n\t"  // getpid is 39 in unistd.h
        "syscall\n\t"
        "mov %%eax, %0\n\t"
        : "=r" (pid)         
        :                    
        : "rax", "rcx", "r11"
    );
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("getpid costs %ld ns through syscall in inline assembly\n", (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec));

    int fd;
    char *filename = "test.txt";
    clock_gettime(CLOCK_MONOTONIC, &start);
    asm volatile(
        "mov $2, %%rax\n\t"  // open is 2 in unistd.h
        "mov $1, %%rdi\n\t"  // 文件名
        "mov $0x40, %%rsi\n\t" // O_CREAT is 0x40
        "syscall\n\t"
        "mov %%eax, %0\n\t"
        : "=r" (fd)          // 输出到fd变量
        : "r" (filename)     // 输入文件名
        : "rax", "rdi", "rsi", "rcx", "r11" // 被破坏的寄存器    
    );
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("open costs %ld ns through syscall in inline assembly\n", (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec));
}

int main() {
    gettime_glibc();
    printf("\n");
    gettime_syscall();
    printf("\n");
    gettime_asm();
    return 0;
}