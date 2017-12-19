//
// Created by simon on 17-12-11.
//

#include "unix_ipc.h"

void unix_fifo() {
    pid_t  p1;
    p1 = fork();
    if (p1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (p1 == 0) {
        ssize_t r;
        int pfd;
        int cfd;
        msg_t fifo_msg;
        void *buff;

        sleep(1);

        fifo_msg.str = (char *)malloc(30 * sizeof(char));
        memset(fifo_msg.str, 0, 30);

        snprintf(fifo_msg.str , 30, "mkfifo_child#%d", getpid());
        fifo_msg.n = getpid();

        // serialization
        buff = malloc(34);
        memset(buff, 0, 34);
        memcpy(buff, fifo_msg.str, 30);
        memcpy(buff + 30, &fifo_msg.n, 4);

        r = mkfifo(fifo_msg.str, OBJ_PERM);
        if (r == -1) {
            perror("child:mkfifo");
            exit(EXIT_FAILURE);
        }

        pfd = open("/tmp/mkfifo_parent", O_WRONLY);
        if (pfd == -1) {
            perror("child:open_pfd");
            exit(EXIT_FAILURE);
        }

        r = write(pfd, buff, 34);
        if (r == -1) {
            perror("child:write");
            exit(EXIT_FAILURE);
        }

        cfd = open(fifo_msg.str, O_RDONLY);
        if (cfd == -1) {
            perror("child:open_cfd");
            exit(EXIT_FAILURE);
        }

        r = read(cfd, &fifo_msg, sizeof(msg_t));
        if (r == -1) {
            perror("child:read");
            exit(EXIT_FAILURE);
        }

        printf("child:fifo_msg.str=%s\n",fifo_msg.str);
        printf("child:fifo_msg.n=%d\n",fifo_msg.n);

        free(fifo_msg.str);
        free(buff);
    } else {
        int pfd;
        int cfd;
        void *buff;
        msg_t *fifo_msg;
        char *cfd_path;
        ssize_t r;

        r = mkfifo("/tmp/mkfifo_parent", OBJ_PERM);
        if (r == -1) {
            perror("parent:mkfifo");
            exit(EXIT_FAILURE);
        }

        pfd = open("/tmp/mkfifo_parent", O_RDONLY);
        if (pfd == -1) {
            perror("parent:open_pfd");
            exit(EXIT_FAILURE);
        }

        fifo_msg = (msg_t *)malloc(sizeof(msg_t));
        fifo_msg->str = (char *)malloc(30 * sizeof(char));
        buff = malloc(34);
        memset(buff, 0, 34);
        memset(fifo_msg->str, 0, 30);

        r = read(pfd, buff, 34);
        if (r == -1) {
            perror("parent:read");
            exit(EXIT_FAILURE);
        }

        memcpy(fifo_msg->str, (char *)buff, 30);
        memcpy(&fifo_msg->n, buff+30, 4);

        printf("parent:fifo_msg.str=%s\n",fifo_msg->str);
        printf("parent:fifo_msg.n=%d\n",fifo_msg->n);


        cfd_path = fifo_msg->str;

        cfd = open(cfd_path, O_WRONLY);
        if (cfd == -1) {
            perror("parent:open_cfd");
            exit(EXIT_FAILURE);
        }

        strcpy(fifo_msg->str, "Hello World");
        fifo_msg->n = 999;

        r = write(cfd, &fifo_msg, sizeof(msg_t));
        if (r == -1) {
            perror("parent:write");
            exit(EXIT_FAILURE);
        }

        wait(&p1);

        unlink("/tmp/mkfifo_parent");
        unlink(cfd_path);

        free(buff);
        free(fifo_msg);
    }
}