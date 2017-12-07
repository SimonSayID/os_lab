//
// Created by ex on 11/29/17.
//


#include "unix_ipc.h"

#define USE_PIPE_PASS_SHM_ID
#define SHM_KEY 0X1234

void system_v_shm() {

#ifdef USE_PIPE_PASS_SHM_ID
    int fd[2];
    pipe(fd);
#else

#endif

    pid_t p1;
    p1 = fork();
    if (p1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (p1 == 0) {

        int shm_id;
        msg_t *shm_ptr;
#ifdef USE_PIPE_PASS_SHM_ID
        close(fd[1]);
        read(fd[0], &shm_id, sizeof(int));
#else
        sleep(3);

        shm_id = shmget(SHM_KEY, 0, 0);
        if (shm_id == -1) {
            perror("shm");
            exit(EXIT_FAILURE);
        }
#endif
        shm_ptr = shmat(shm_id, NULL, SHM_RDONLY);
        if (shm_ptr == (void *)-1) {
            perror("child shm_at");
            exit(EXIT_FAILURE);
        }

        printf("child shm_ptr:%p\n", (void *)shm_ptr);
        printf("child read from shm:%s\n",shm_ptr->str);
        printf("child read from shm:%d\n",shm_ptr->n);

        shmdt(shm_ptr);

    } else {

        int shm_id;
        msg_t *shm_ptr;

        shm_id = shmget(SHM_KEY, sizeof(msg_t), IPC_CREAT | OBJ_PERM);
        if (shm_id == -1) {
            perror("shm");
            exit(EXIT_FAILURE);
        }
        shm_ptr = shmat(shm_id, NULL, 0);
        if (shm_ptr == (void *)-1) {
            perror("parent shm_at");
            exit(EXIT_FAILURE);
        }

        shm_ptr->str = "This is the msg.";
        shm_ptr->n = 99;

#ifdef USE_PIPE_PASS_SHM_ID
        close(fd[0]);
        write(fd[1], &shm_id, sizeof(int));
#else

#endif
        printf("parent shm_ptr:%p\n", (void *)shm_ptr);
        printf("parent:%s\n",shm_ptr->str);
        printf("parent:%d\n",shm_ptr->n);

        wait(&p1);
        shmdt(shm_ptr);
        shmctl(shm_id, IPC_RMID, 0);
    }


}