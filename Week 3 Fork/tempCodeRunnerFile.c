#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("before child process \n");
    int pid = fork();
    if (pid < 0)
    {
        printf("Error \n");
    }
    else if (pid == 0)
    {
        // executing in child process
        for (int i = 0; i < 100; i++)
        {
            printf("Iam child process \n");
        }
    }
    else
    {
        for (int i = 0; i < 100; i++)
        {
            printf("Iam parent process \n");
        }
    }
    printf("after child process \n");
    sleep(1);
    return 0;
    // after fork(), process turns into 2 process. After that, both parent and child process run
}