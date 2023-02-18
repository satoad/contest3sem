#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

enum { BASE = 10 };

struct MsgBuf
{
    long mtype;
    long long x1;
    long long x2;
};

int
main(int argc, char *argv[])
{
    key_t ipc_key = (key_t) strtol(argv[1], NULL, BASE);
    int n = strtol(argv[2], NULL, BASE);
    long long int value1 = strtoll(argv[3], NULL, BASE);
    long long int value2 = strtoll(argv[4], NULL, BASE);
    long long int maxval = strtoll(argv[5], NULL, BASE);

    int msg_id = msgget(ipc_key, IPC_CREAT | IPC_EXCL | 0666);
    if (msg_id < 0) {
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        int pid;
        if ((pid = fork()) == 0) {
            while(1) {
                struct MsgBuf msg;
                int r = msgrcv(msg_id, &msg, sizeof(msg) - sizeof(long), i + 1, 0);
                if (r < 0) {
                    _exit(0);
                }

                long long int x3 = msg.x1 + msg.x2;
                printf("%d %lld\n", i, x3);
                fflush(stdout);

                if (x3 > maxval) {
                    msgctl(msg_id, IPC_RMID, 0);
                    _exit(0);
                }

                msg.mtype = x3 % n + 1;
                msg.x1 = msg.x2;
                msg.x2 = x3;
                msgsnd(msg_id, &msg, sizeof(msg) - sizeof(long), 0);
            }
            _exit(0);
            } else if (pid < 0) {
                msgctl(msg_id, IPC_RMID, 0);
                while(wait(NULL) > 0);
                return 1;
            }
        }


    struct MsgBuf first;
    first.mtype = 1;
    first.x1 = value1;
    first.x2 = value2;
    msgsnd(msg_id, &first, sizeof(first) - sizeof(long), 0);

    while(wait(NULL) > 0);
    return 0;
}