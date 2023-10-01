#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define SHM_NAME "a"

int main() {
    printf("Before creating child process\n");

    int pid = fork();
    if (pid < 0) {
        printf("Error\n");

    } else if (pid == 0) {
        printf("I am child process\n");

        int sid = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        printf("Child sid: %d\n", sid);

        ftruncate(sid, sizeof(int));

        int* p = (int*)mmap(NULL, sizeof(int), PROT_WRITE, MAP_SHARED, sid, 0);

        *p = 100;

        printf("Child process terminated\n");

    } else {
        printf("I am parent process\n");

        wait(NULL); // blocking

        int sid = shm_open(SHM_NAME, O_RDWR, 0666);
        printf("Parent sid: %d\n", sid);

        int* p = (int*)mmap(NULL, sizeof(int), PROT_READ, MAP_SHARED, sid, 0);

        printf("%d\n", *p);

        *p = 200;

        shm_unlink(SHM_NAME);
    
    }


    return 0;
}