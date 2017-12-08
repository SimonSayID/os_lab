//
// Created by ex on 11/29/17.
//


/*
 * POSIX SEM (name)
 * */

#include "unix_sync.h"

void posix_sem_n() {
    pid_t p1;
    p1 = fork();
    if (p1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (p1 == 0) {
        int *w;
        int r;
        sleep(2);
        sem_t *sem;
        sem = sem_open("psn", 0);
        w = (int *) malloc(sizeof(int));
        r = sem_getvalue(sem, w);
        if (r != 0) {
            perror("sem_getvalue");
        }
        printf("child:current sem value: %d\n", *w);
        sem_wait(sem);
        printf("child:got the sem\n");
        sleep(2);
        printf("child:releasing the sem\n");
        sem_post(sem);
    } else {
        sem_t *sem;
        int *w;
        int r;
        sem = sem_open("psn", O_CREAT, OBJ_PERM, 1);
        if (sem == SEM_FAILED) {
            perror("sem_open psn");
            exit(EXIT_FAILURE);
        }
        w = (int *) malloc(sizeof(int));
        r = sem_getvalue(sem, w);
        if (r != 0) {
            perror("parent:sem_getvalue");
        }
        printf("parent:current sem value: %d\n", *w);
        sem_wait(sem);
        printf("parent:got the sem\n");
        sleep(2);
        printf("parent:releasing the sem\n");
        sem_post(sem);
        wait(&p1);
    }

}