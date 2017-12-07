//
// Created by ex on 11/29/17.
//

#include "unix_sync.h"

pthread_mutex_t m_lock = PTHREAD_MUTEX_INITIALIZER;

int sync_num = 0;

void* get_lock_and_add(void *arg) {
    pthread_mutex_lock(&m_lock);
    printf("add: got the lock\n");
    int times = 5;
    while (times > 0) {
        sync_num = sync_num + 1;
        times--;
    }
    printf("add: releasing the lock\n");
    pthread_mutex_unlock(&m_lock);
}

void* get_lock_and_reset(void *arg) {
    pthread_mutex_lock(&m_lock);
    printf("reset: got the lock\n");
    sync_num = 0;
    printf("reset: releasing the lock\n");
    pthread_mutex_unlock(&m_lock);
}

void posix_mutex() {
    int r1, r2;
    pthread_t t1, t2;
    r1 = pthread_create(&t1, NULL, get_lock_and_add, NULL);
    if (r1 != 0 ) {
        perror("t1");
    }
    r2 = pthread_create(&t2, NULL, get_lock_and_reset, NULL);
    if (r2 != 0 ) {
        perror("t2");
    }
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("number = %d\n",sync_num);
}