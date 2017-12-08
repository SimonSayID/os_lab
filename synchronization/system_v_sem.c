//
// Created by ex on 12/6/17.
//

#include "unix_sync.h"

#define SV_SEM_KEY 0X123

union semun {
    int val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array;  /* Array for GETALL, SETALL */
    struct seminfo *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};

void system_v_sem() {
    int perms;
    int sem_id;
    pid_t p1;
    perms = S_IRUSR | S_IWUSR;

    p1 = fork();
    if (p1 == -1) {
        perror("fork");
    } else if (p1 == 0) {
        sleep(2);
        struct sembuf sops;
        sem_id = semget(SV_SEM_KEY, 0, perms);
        sops.sem_num = 0;
        sops.sem_op = -1;
        sops.sem_flg = 0;
        semop(sem_id, &sops, 1);
        for (int i = 0; i < 3; ++i) {
            printf("child i = %d\n", i);
        }
        sops.sem_num = 0;
        sops.sem_op = 1;
        sops.sem_flg = 0;
        semop(sem_id, &sops, 1);
    } else {
        int r;
        union semun arg;
        struct sembuf sops;
        sem_id = semget(SV_SEM_KEY, 1, IPC_CREAT | IPC_EXCL | perms);
        if (sem_id == -1) {
            perror("sem_get");
            exit(EXIT_FAILURE);
        }
        arg.val = 1;
        r = semctl(sem_id, 0, SETVAL, arg);
        if (r == -1) {
            perror("sem_ctl SETVAL");
            exit(EXIT_FAILURE);
        }
        sops.sem_num = 0;
        sops.sem_op = -1;
        sops.sem_flg = 0;
        semop(sem_id, &sops, 1);
        for (int i = 0; i < 3; ++i) {
            printf("parent i = %d\n", i);
        }
        sops.sem_num = 0;
        sops.sem_op = 1;
        sops.sem_flg = 0;
        semop(sem_id, &sops, 1);
        wait(&p1);
        semctl(sem_id, 0, IPC_RMID);
    }
}