#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

typedef struct Adder 
{
    int operand1, operand2, result;
    int ready;
} Adder;

void *add(void *arg) {
    Adder *adder = (Adder *) arg;
    while (1) {
        if (adder->ready == 1) {
            adder->result = adder->operand1 + adder->operand2; 
            adder->ready = 0; 
            break; 
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    Adder adder;
    adder.ready = 0;

    printf("Enter operand1: "); scanf("%d", &(adder.operand1));
    printf("Enter operand2: "); scanf("%d", &(adder.operand2));
    adder.ready = 1;

    pthread_t t;
    pthread_create(&t, NULL, add, &adder);
    while (1){
        if (adder.ready == 0){
            printf("Result of sum : %d\n", adder.result);
            break; 
        }
    }
    pthread_join(t, NULL);
    return 0;
}

