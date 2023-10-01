#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    printf("before child process \n");
    int x = 10;
    int pid = fork();
    if (pid < 0)
    {
        printf("Error \n");
    }
    else if (pid == 0)
    {
        // executing in child process
        while (1)
        {
        printf("Iam child process \n");
        x = 20;
        printf("Child process terminated: %d\n", x);
        printf("Child %x: ", &x);
        sleep(1);
        }
    }

    else
    {
        while (1)
        // wait(NULL);
        {
            printf("Iam parent process: %d \n", x);
            printf("Parent %x: ", &x);
            printf("Parent process waiting\n");
            sleep(1);
        }
    }
    // printf("after child process \n");
    // sleep(1);
    return 0;
    // after fork(), process turns into 2 process. After that, both parent and child process run
}