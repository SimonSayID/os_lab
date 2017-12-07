//
// Created by ex on 11/30/17.
//

#include "unix_sync.h"

typedef struct mq_s mq_t;

struct mq_s {
    mq_t        *next;
    int         data;
};

mq_t *wq;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t c_lock = PTHREAD_MUTEX_INITIALIZER;

void* process_msg(void *arg) {
    mq_t *mp;
    for ( ; ; ) {
        pthread_mutex_lock(&c_lock);
        printf("process_msg: got the lock\n");
        while (wq == NULL) {
            pthread_cond_wait(&cond, &c_lock);
        }
        printf("process_msg: got the cond\n");
        mp = wq;
        wq = mp->next;
        printf("process_msg: releasing the lock\n");
        pthread_mutex_unlock(&c_lock);
        mp->data += 1;
        printf("process_msg: msg data = %d\n",mp->data);
    }
}

void* eq_msg(void *arg) {

    for (int i = 1; i < 5; ++i) {
        int r = 0;
        pthread_mutex_lock(&c_lock);
        printf("eq_msg: got the lock\n");
        mq_t *mp = (mq_t *)malloc(sizeof(mq_t));
        mp->data = i;
        mp->next = wq;
        wq = mp;
        printf("eq_msg: releasing the lock\n");
        pthread_mutex_unlock(&c_lock);
        printf("eq_msg: signal the cond\n");
        pthread_cond_signal(&cond);
        r = sched_yield();
        if (r != 0) {
            perror("sched_yield");
        }
    }

}

void posix_cond() {

    int r1, r2;
    pthread_t t1, t2;

    wq = (mq_t *)malloc(sizeof(mq_t));
    wq->data = 0;
    wq->next = NULL;
    r1 = pthread_create(&t1, NULL, process_msg, NULL);
    r2 = pthread_create(&t2, NULL, eq_msg, NULL);
    if (r1 != 0) {
        perror("pthread_create");
    }
    if (r2 != 0) {
        perror("pthread_create");
    }
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}