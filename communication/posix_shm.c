//
// Created by simon on 17-12-10.
//

#include "unix_ipc.h"

void posix_shm() {
    int fd[2];
    pid_t p1;

    pipe(fd);
    p1 = fork();
    if (p1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (p1 == 0) {
        int f;
        size_t l;
        ssize_t r;
        char *name;
        msg_t p_msg;
        void *address;

        close(fd[1]);

        name = (char *)malloc(5);
        r = read(fd[0], name, 5);
        if (r == -1) {
            perror("child:read");
            exit(EXIT_FAILURE);
        }

        f = shm_open(name, O_RDWR, 0);
        if (f == -1) {
            perror("child:shm_open");
            exit(EXIT_FAILURE);
        }

        p_msg.str = "hello world";
        p_msg.n = 11;
        l = sizeof(msg_t);

        r = ftruncate(f, l);
        if (r == -1) {
            perror("child:ftruncate");
            exit(EXIT_FAILURE);
        }

        address = mmap(NULL, l, PROT_WRITE, MAP_SHARED, f, 0);
        if (address == MAP_FAILED) {
            perror("child:mmap");
            exit(EXIT_FAILURE);
        }

        memcpy(address, &p_msg, l);

        r = msync(address, l, MS_SYNC);
        if (r == -1) {
            perror("child:msync");
            exit(EXIT_FAILURE);
        }

        shm_unlink("/pshm");
    } else {
        int f;
        ssize_t r;
        msg_t p_msg;
        void *address;

        close(fd[0]);
        f = shm_open("/pshm", O_CREAT | O_EXCL | O_RDWR, OBJ_PERM);
        if (f == -1) {
            perror("parent:shm_open");
            exit(EXIT_FAILURE);
        }
        address = mmap(NULL, 1, PROT_READ, MAP_SHARED, f, 0);
        if (address == MAP_FAILED) {
            perror("parent:mmap");
            exit(EXIT_FAILURE);
        }

        r = write(fd[1], "/pshm", 5);
        if (r == -1) {
            perror("parent:read");
            exit(EXIT_FAILURE);
        }

        wait(&p1);

        p_msg = *((msg_t *)address);
        printf("parent:p_msg.str=%s\n",p_msg.str);
        printf("parent:p_msg.n=%d\n",p_msg.n);

        shm_unlink("/pshm");
    }
}