#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int a[10] = {1,2,3,4,5,6,7,8,9,10};
    pid_t pid = fork();
    if (pid < 0)
    {
        printf("fork failed\n");
        return 1;
    }
    if (pid == 0)
    {
        printf("sum of a[0] to a[9] is %d\n", a[0]+a[1]+a[2]+a[3]+a[4]+a[5]+a[6]+a[7]+a[8]+a[9]);
    }
    else
    {
        wait(NULL);
        printf("parent process finishes\n");
    }
    return 0;
}