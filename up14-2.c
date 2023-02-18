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
    int count = strtol(argv[1], NULL, BASE);
    key_t ipc_key = (key_t) strtol(argv[2], NULL, BASE);
    int nproc = strtol(argv[3], NULL, BASE);
    int iter_count = strtol(argv[4], NULL, BASE);

    int shm_id = shmget(ipc_key, sizeof(int) * count, IPC_CREAT | IPC_EXCL | 0666);
    if (shm_id < 0) {
        exit(1);
    }

    int *mem_ptr = shmat(shm_id, NULL, 0);
    if (mem_ptr == (void*) -1) {
        exit(2);
    }

    for (int i = 0; i < count; i++) {
        scanf("%d", &mem_ptr[i]);
    }

    int sem_id = semget(ipc_key, count, IPC_CREAT | IPC_EXCL | 0666);
    if (sem_id < 0) {
        exit(3);
    }

    unsigned short *sctl_arg = calloc(count, sizeof(*sctl_arg));
    for (int i = 0; i < count; i++) {
        sctl_arg[i] = 1;
    }

    if (semctl(sem_id, 0, SETALL, sctl_arg) < 0) {
        exit(4);
    }

    free(sctl_arg);

    for (int i = 0; i < nproc; i++) {
        int pid;
        if ((pid = fork()) == 0) {
            unsigned int rand_num = strtoul(argv[5 + i], NULL, BASE);
            srand(rand_num);
            struct sembuf wait[] =
                    {
                            { .sem_num = i, .sem_op = -1, .sem_flg = 0 },
                            { .sem_num = i, .sem_op = -1, .sem_flg = 0 },
                    };

            struct sembuf unlock[] =
                    {
                            { .sem_num = i, .sem_op = 1, .sem_flg = 0 },
                            { .sem_num = i, .sem_op = 1, .sem_flg = 0 },
                    };

            for (int j = 0; j < iter_count; j++) {
                unsigned ind1 = rand() % count;
                unsigned ind2 = rand() % count;
                int value = rand() % 10;

                wait[0].sem_num = ind1;
                wait[1].sem_num = ind2;
                unlock[0].sem_num = ind1;
                unlock[1].sem_num = ind2;

                if (ind1 != ind2) {

                    if (semop(sem_id, wait, 2) < 0) {
                        _exit(0);
                    }
                } else {
                    if (semop(sem_id, wait, 1) < 0) {
                        _exit(0);
                    }
                }

                operation(mem_ptr, ind1, ind2, value);

                if (ind1 != ind2) {
                    if (semop(sem_id, unlock, 2) < 0) {
                        _exit(0);
                    }
                } else {
                    if (semop(sem_id, unlock, 1) < 0) {
                        _exit(0);
                    }
                }
            }
            _exit(0);
        }
    }

    while (wait(NULL) > 0);

    for (int i = 0; i < count; i++) {
        printf("%d\n", mem_ptr[i]);
        fflush(stdout);
    }

    semctl(sem_id, 0, IPC_RMID);
    shmdt(mem_ptr);
    shmctl(shm_id, IPC_RMID, 0);

    return 0;
}