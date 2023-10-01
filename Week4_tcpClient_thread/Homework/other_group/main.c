#include <stdio.h>
#include <pthread.h>

typedef struct {
    int x, y, z;
    int ready;
} shared_data;

shared_data data = {0, 0, 0, 0};

void *child_thread(void *arg) {
    while (data.ready == 0) {
        // Chờ đến khi ready == 1
    }
    data.z = data.x + data.y;
    data.ready = 0;
    return NULL;
}

int main() {
    // Tạo thread con
    pthread_t thread;
    pthread_create(&thread, NULL, child_thread, NULL);

    // Thread mẹ
    printf("Nhập giá trị x: ");
    scanf("%d", &data.x);
    printf("Nhập giá trị y: ");
    scanf("%d", &data.y);
    data.ready = 1;
    while (data.ready == 1) {
        // Chờ đến khi ready == 0
    }
    printf("Giá trị z = %d\n", data.z);

    pthread_join(thread, NULL);

    return 0;
}