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
    setbuf(stdin, NULL);
    int count = strtol(argv[1], NULL, BASE);

    int rand_num;
    int fp1 = open("/dev/urandom", O_RDONLY);
    if (fp1 < 0) {
        exit(1);
    }
    read(fp1, &rand_num, sizeof(rand_num));
    close(fp1);
    key_t ipc_key = ftok(".", rand_num);

    int sem_id = semget(ipc_key, count, IPC_CREAT | IPC_EXCL | 0666);
    if (sem_id < 0) {
        exit(2);
    }

    if (semctl(sem_id, 0, SETVAL, 1) < 0) {
        exit(3);
    }

    for (int i = 0; i < count; i++) {
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

                int num;
                if (scanf("%d", &num) == 1) {
                    printf("%d %d\n", i, num);
                    fflush(stdout);
                } else {
                    semctl(sem_id, count, IPC_RMID, 0);
                    _exit(0);
                }
                
                unlock[0].sem_num =  (count + (num % count)) % count;

                if (semop(sem_id, unlock, 1) < 0) {
                    break;
                }
            }
            _exit(0);
        }
    }
    
    while(wait(NULL) > 0);
    return 0;
}