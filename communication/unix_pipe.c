//
// Created by ex on 11/29/17.
//

#include "unix_ipc.h"

void unix_pipe() {

    int r;
    ssize_t n;
    int fd[2];
    pid_t p1;
    char msg[11];

    r = pipe(fd);
    if (r == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    p1 = fork();
    if (p1 == -1) {
        perror("fork");
    } else if (p1 == 0) {
        close(fd[0]);
        printf("child writing\n");
        write(fd[1], "From Child\n", 11);
    } else {
        close(fd[1]);
        printf("parent reading\n");
        n = read(fd[0], msg, 11); //blocked
        if (n == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        write(STDOUT_FILENO, msg, (size_t)n);
        wait(&p1);
    }

}