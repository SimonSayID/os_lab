//
// Created by ex on 11/29/17.
//


/*
 * POSIX SEM
 * */

#include "unix_sync.h"

sem_t sem;

int sem_res = 0;

void* get_sem(void *arg) {
    int *w = (int *)arg;
    int r;
    r = sem_getvalue(&sem, w);
    if (r != 0) {
        perror("sem_getvalue");
    }
    printf("current sem value: %d\n", *w);
    sem_wait(&sem);
    printf("thread's id: %lu\n",syscall(__NR_gettid));
    printf("got the sem\n");
    sleep(2);
    printf("use the res %d\n",sem_res);
    printf("releasing the sem\n");
    sem_post(&sem);
}

void posix_sem() {

    int r1, r2, r3, s;
    pthread_t t1, t2, t3;
    int *w = (int *)malloc(sizeof(int));
    s = sem_init(&sem, 0, 2);
    if (s != 0) {
        perror("sem_init");
    }
    r1 = pthread_create(&t1, NULL, get_sem, (void *)w);
    if (r1 != 0) {
        perror("t1");
    }
    r2 = pthread_create(&t2, NULL, get_sem, (void *)w);
    if (r2 != 0) {
        perror("t2");
    }
    r3 = pthread_create(&t3, NULL, get_sem, (void *)w);
    if (r3 != 0) {
        perror("t3");
    }
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    free(w);
}