//
// Created by ex on 11/29/17.
//

#ifndef OS_LAB_MIAN_H
#define OS_LAB_MIAN_H

#define OBJ_PERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

#include <stdio.h>
#include <sched.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h> // system v
#include <sys/stat.h>
#include <sys/wait.h>
#include <semaphore.h> //posix
#include <sys/param.h>
#include <sys/types.h>
#include <sys/syscall.h>


#endif //OS_LAB_MIAN_H
