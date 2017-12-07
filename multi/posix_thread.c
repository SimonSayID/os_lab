//
// Created by ex on 11/29/17.
//

#include "unix_multi.h"

void* print_thread_info(void *arg){
    printf("new thread's id: %lu\n",syscall(__NR_gettid));
}

void posix_thread() {
    int result;
    pthread_t tid;
    printf("pid is %d\n", getpid());
    printf("thread id: %lu (same as pid)\n",syscall(__NR_gettid));
    result = pthread_create(&tid, NULL, print_thread_info, NULL);
    if (result != 0) {
        perror("pthread_create error");
    } else {
        pthread_join(tid, NULL);
    }
}