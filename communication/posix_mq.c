//
// Created by simon on 17-12-10.
//

#include "unix_ipc.h"

void posix_mq() {

    pid_t p1;
    p1 = fork();
    if (p1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (p1 == 0) {
        sleep(2);
        int r;
        mqd_t mqd;
        struct mq_attr mq_a;
        mqd = mq_open("/pmq", O_WRONLY);
        if (mqd == (mqd_t)-1) {
            perror("child:mq_open");
            exit(EXIT_FAILURE);
        }
        r = mq_getattr(mqd, &mq_a);
        if (r == -1) {
            perror("child:mq_getattr");
            exit(EXIT_FAILURE);
        }
        printf("child: mq_a.mq_maxmsg %ld\n", mq_a.mq_maxmsg);
        printf("child: mq_a.mq_msgsize %ld\n", mq_a.mq_msgsize);
        printf("child: mq_a.mq_curmsgs %ld\n", mq_a.mq_curmsgs);
        r = mq_send(mqd, "hello 999", 9, 10);
        if (r == -1) {
            perror("child:mq_send");
            exit(EXIT_FAILURE);
        }
    } else {
        mqd_t mqd;
        struct mq_attr mq_a;
        ssize_t n;
        char *buff;
        unsigned int pri;
        mq_a.mq_flags = 0;
        mq_a.mq_maxmsg = 2;
        mq_a.mq_msgsize = 1024;
        mqd = mq_open("/pmq", O_CREAT | O_EXCL, OBJ_PERM, &mq_a);
        if (mqd == (mqd_t)-1) {
            perror("parent:mq_open");
            exit(EXIT_FAILURE);
        }
        buff = (char *)malloc((size_t)mq_a.mq_msgsize);
        n = mq_receive(mqd, buff, (size_t)mq_a.mq_msgsize, &pri);
        if (n == -1) {
            perror("parent:mq_receive");
            exit(EXIT_FAILURE);
        }
        printf("parent:message from child(content):%s\n",buff);
        printf("parent:message from child(priority):%u\n", pri);
        printf("parent:message from child(bytes):%ld\n", n);

        wait(&p1);
        n = mq_unlink("/pmq");
        if (n == -1) {
            perror("parent:mq_unlink");
            exit(EXIT_FAILURE);
        }
    }

}

