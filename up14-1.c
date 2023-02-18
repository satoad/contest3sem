#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <limits.h>
#include <sys/stat.h>
#include <stdint.h>
#include <sys/sem.h>
#include <sys/shm.h>

enum { BASE = 10 };

int
main(int argc, char *argv[])
{
    int nproc = strtol(argv[1], NULL, BASE);
    key_t ipc_key = (key_t) strtol(argv[2], NULL, BASE);
    int maxval = strtol(argv[3], NULL, BASE);

    int shm_id = shmget(ipc_key, sizeof(int) * 2, IPC_CREAT | IPC_EXCL | 0666);
    if (shm_id < 0) {
        exit(1);
    } 

    int *mem_ptr = shmat(shm_id, NULL, 0);

    if (mem_ptr == (void*) -1) {
        exit(2);
    }

    mem_ptr[0] = 0;
    mem_ptr[1] = 0;

    int sem_id = semget(ipc_key, nproc, IPC_CREAT | IPC_EXCL | 0666);
    if (sem_id < 0) {
        exit(3);
    }

    if (semctl(sem_id, 0, SETVAL, 1) < 0) {
        exit(4);
    }

    for (int i = 0; i < nproc; i++) {
        int pid;
        if ((pid = fork()) == 0) {
            struct sembuf wait[] =
            {
                { .sem_num = i, .sem_op = -1, .sem_flg = 0 },
            };

            struct sembuf unlock[] = 
            {
                { .sem_num = i, .sem_op = 1, .sem_flg = 0 },
            };

            while(1) {
                if (semop(sem_id, wait, 1) < 0) {
                    _exit(0);
                }

                printf("%d %d %d\n", i + 1, mem_ptr[0], mem_ptr[1]);
                fflush(stdout);

                mem_ptr[0]++;
                mem_ptr[1] = i + 1;

                if(mem_ptr[0] > maxval) {
                    semctl(sem_id, 0, IPC_RMID);
                    _exit(0);
                }
                
                unsigned tmp = mem_ptr[0] % nproc;
                unlock[0].sem_num = (((((tmp * tmp) % nproc) * tmp) % nproc) * tmp) % nproc;

                if (semop(sem_id, unlock, 1) < 0) {
                    break;
                }
            }
            _exit(0);
        }
    }
    while(wait(NULL) > 0);

    shmdt(mem_ptr);
    shmctl(shm_id, IPC_RMID, 0);

    return 0;
}