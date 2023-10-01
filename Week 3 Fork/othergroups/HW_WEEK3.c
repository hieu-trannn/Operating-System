/*
If you cannot run file, please restart PC and try again
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// define struct
struct shared_data
{
    int x;
    int y;
    int z;
    int ready;
};

int main()
{
    // create common memory
    key_t key = 1234;
    int shmid = shmget(key, sizeof(struct shared_data), IPC_CREAT | 0666);

    struct shared_data *data = shmat(shmid, NULL, 0);
    // set ready =0
    data->ready = 0;

    int pid = fork();
    // pid_t pid = fork();

    // check if fork successfully
    if (pid < 0)
    {
        printf("fork failed \n");
        printf("Try again");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    { // child process
        while (data->ready == 0)
        {
            // sleep until parent process set ready to 1
            sleep(1);
        }

        // count z
        data->z = data->x + data->y;
        // finish counting z, set ready to 0
        data->ready = 0;

        exit(EXIT_SUCCESS);
    }
    else
    { // parent process
        printf("Enter the value of x: ");
        scanf("%d", &data->x);

        printf("Enter the value of y: ");
        scanf("%d", &data->y);

        // set ready = 1
        data->ready = 1;

        while (data->ready != 0)
        {
            // sleep until child process set ready to 0
            sleep(1);
        }
        // print z when ready is 0
        printf("The value of z is %d\n", data->z);

        shmdt(data);
        // delete common memory
        shmctl(shmid, IPC_RMID, NULL);

        exit(EXIT_SUCCESS);
    }

    return 0;
}
