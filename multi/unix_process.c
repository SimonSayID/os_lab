//
// Created by ex on 11/29/17.
//

#include "unix_multi.h"

void unix_process() {

    pid_t pid;
    pid = fork();
    if (pid == -1) {
        perror("fork error");
    } else if (pid == 0) {
        printf("child %d\n",getpid());
    } else {
        printf("parent %d\n", getpid());
        wait(&pid);
    }

}