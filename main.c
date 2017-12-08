
#include "main.h"

#include "communication/unix_ipc.h"
#include "multi/unix_multi.h"
#include "synchronization/unix_sync.h"

int main() {
//    unix_process();
//    posix_thread();
//    posix_mutex();
//    posix_cond();
//    posix_sem();
//    posix_bari();
//    unix_pipe();
//    system_v_shm();
//    system_v_mq();
//    system_v_sem();
    posix_sem_n();
    printf("back to main\n");
    exit(EXIT_SUCCESS);
}