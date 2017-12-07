//
// Created by ex on 11/29/17.
//

#include "unix_ipc.h"

typedef struct mq_s mq_t;

struct mq_s {
    long type;
    msg_t data;
};

void system_v_mq() {

    pid_t p1;
    int msqid;
    msqid = msgget(IPC_PRIVATE, IPC_CREAT | OBJ_PERM);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    p1 = fork();
    if (p1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (p1 == 0) {
        int r;
        mq_t msg;
        msg.type = 10;
        msg.data.str = "hello from other side";
        msg.data.n = 22;

        r = msgsnd(msqid, &msg, (size_t)msg.data.n, 0);
        if (r == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

    } else {
        ssize_t l;
        mq_t msg;

        wait(&p1);

        l = msgrcv(msqid, &msg, 22, 10, 0);
        if (l == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        printf("parent rcv:type = %ld\n", msg.type);
        printf("parent rcv:data.str = %s\n", msg.data.str);
        printf("parent rcv:data.n = %d\n", msg.data.n);

        if (msgctl(msqid, IPC_RMID, NULL) == -1) {
            perror("msgctl_IPC_RMID");
            exit(EXIT_FAILURE);
        }
    }

}