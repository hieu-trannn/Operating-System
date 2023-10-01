#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    fork();
    fork();
    fork();
    printf("Hello \n");
    return 0;
}