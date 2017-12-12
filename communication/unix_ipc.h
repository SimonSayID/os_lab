//
// Created by ex on 11/29/17.
//

#ifndef OS_LAB_OB_IPC_H
#define OS_LAB_OB_IPC_H

#include "main.h"

typedef struct msg_s msg_t;

struct msg_s {
    char *str;
    int n;
};

void unix_pipe();

void unix_fifo();

void system_v_shm();

void system_v_mq();

void posix_mq();

void posix_shm();

#endif //OS_LAB_OB_IPC_H
