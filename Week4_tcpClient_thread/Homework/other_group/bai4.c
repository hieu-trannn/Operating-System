#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int x;
    int y;
    int z;
    int ready;
} shared_data;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
/*
void* thread_func(void* arg) {
    shared_data data = *(shared_data*) arg;

    // Tính z
    data.z = data.x + data.y;

    // Gửi tín hiệu ready = 1 cho main thread
    pthread_mutex_lock(&mutex);
    data.ready = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}
*/

void* thread_func(void* arg) {
    shared_data *data = (shared_data*) arg;

    // Tính z
    data->z = data->x + data->y;

    // Gửi tín hiệu ready = 1 cho main thread
    pthread_mutex_lock(&mutex);
    data->ready = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}


int main() {
    shared_data data = {0, 0, 0, 0};

    // Tạo thread
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, thread_func, (void*) &data) != 0) {
        fprintf(stderr, "Thread creation failed\n");
        return 1;
    }


    // Nhập giá trị x, y từ bàn phím và gán ready = 1
    printf("Enter x: ");
    scanf("%d", &data.x);
    printf("Enter y: ");
    scanf("%d", &data.y);
    data.ready = 1;

    // Chờ đến khi ready == 0
    /* pthread_mutex_lock(&mutex);
    while (data.ready != 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex); */

    // Chờ đến khi ready == 0
    pthread_mutex_lock(&mutex);
    while (data.ready != 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);


    // In giá trị z và kết thúc
    printf("z = %d\n", data.z);

    pthread_join(thread_id, NULL); // Đợi thread con kết thúc
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
