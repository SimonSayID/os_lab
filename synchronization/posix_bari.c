//
// Created by ex on 11/29/17.
//

#include "unix_sync.h"

pthread_barrier_t barrier;

void* mul_num1(void *arg) {
    int *num = (int *)arg;
    for (int i = 0; i < 3; ++i) {
        *num *= 2;
    }
    printf("thread %lu done the job, num1 = %d\n",
           syscall(__NR_gettid), *num);
    pthread_barrier_wait(&barrier);
}

void* mul_num2(void *arg) {
    int *num = (int *)arg;
    for (int i = 0; i < 3; ++i) {
        *num *= 4;
    }
    printf("thread %lu done the job, num2 = %d\n",
           syscall(__NR_gettid), *num);
    pthread_barrier_wait(&barrier);
}

void posix_bari() {
    int *num1, *num2;
    int r1, r2;
    pthread_t t1, t2;
    num1 = (int *)malloc(sizeof(int));
    *num1 = 1;
    num2 = (int *)malloc(sizeof(int));
    *num2 = 2;
    pthread_barrier_init(&barrier, NULL, 3);
    r1 = pthread_create(&t1, NULL, mul_num1, num1);
    if (r1 != 0) {
        perror("t1");
    }
    r2 = pthread_create(&t2, NULL, mul_num2, num2);
    if (r2 != 0) {
        perror("t2");
    }
    pthread_barrier_wait(&barrier);
    printf("num = %d\n",*num2 / *num1);
    free(num1);
    free(num2);
}