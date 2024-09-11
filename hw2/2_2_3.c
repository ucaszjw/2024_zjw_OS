#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define LOOP 10

int main(int argc, char *argv[])
{
    pid_t pid;
    int loop;
    for (loop = 0; loop < LOOP; loop++)
    {
        if ((pid = fork()) < 0)
            fprintf(stderr, "fork failed\n");
        else if (pid == 0)
        {
            printf(" I am child process\n");
            break;
        }
        else
        {
            sleep(5);
        }
    }
    return 0;
}
