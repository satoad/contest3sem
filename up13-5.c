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

enum
{
    FLAG_ALRM = 1,
    FLAG_IO,
    FLAG_USR1,
    FLAG_USR2
};

volatile sig_atomic_t flag = 0;

unsigned char
byte_collect(unsigned char byte, unsigned char new_bite)
{
    byte <<= 1;
    byte |= new_bite;
    return byte;
}

void
handler(int signal) {
    if (signal == SIGALRM) {
        flag = FLAG_ALRM;
    } else if (signal == SIGIO) {
        _exit(0);
    } else if (signal == SIGUSR1) {
        flag = FLAG_USR1;
    } else if (signal == SIGUSR2) {
        flag = FLAG_USR2;
    }
}

int
main(int argc, char *argv[])
{
    sigset_t s1, s2;
    sigemptyset(&s1);
    sigemptyset(&s2);
    sigaddset(&s1, SIGUSR1);
    sigaddset(&s1, SIGUSR2);
    sigaddset(&s1, SIGALRM);
    sigaddset(&s1, SIGIO);
    sigprocmask(SIG_BLOCK, &s1, NULL);


    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGUSR2, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGIO, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGALRM, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    int fd[2];
    pipe(fd);
    int pid1;
    if ((pid1 = fork()) == 0) {
        close(fd[1]);
        int pid;
        read(fd[0], &pid, sizeof(pid));
        close(fd[0]);

        int num = 0;
        unsigned char byte = 0;
        while (1) {
            while (!flag) {
                sigsuspend(&s2);
            }

            if (flag == FLAG_USR1) {
                num++;
                byte = byte_collect(byte, 1);
            }

            if (flag == FLAG_USR2) {
                num++;
                byte = byte_collect(byte, 0);
            }

            if (num == CHAR_BIT) {
                printf("%c", byte);
                fflush(stdout);
                byte = 0;
                num = 0;
            }

            flag = 0;
            kill(pid, SIGALRM);
        }
    } else if (pid1 < 0) {
        exit(1);
    } else {
        close(fd[0]);

        int pid2;
        if ((pid2 = fork()) == 0) {
            pid2 = getpid();
            write(fd[1], &pid2, sizeof(pid2));
            close(fd[1]);

            int fp1 = open(argv[1], O_RDONLY);

            unsigned char byte;
            while (read(fp1, &byte, sizeof(byte)) == sizeof(byte)) {
                for (int i = CHAR_BIT - 1; i >= 0; i--) {
                    if ((byte >> i) & 1) {
                        kill(pid1, SIGUSR1);
                    } else {
                        kill(pid1, SIGUSR2);
                    }

                    while (!flag) {
                        sigsuspend(&s2);
                    }
                    flag = 0;
                }
            }

            close(fp1);
            kill(pid1, SIGIO);
            _exit(0);
        } else if (pid2 < 0) {
            close(fd[1]);
            return 1;
        } else {
            close(fd[1]);

            while (wait(NULL) > 0);
            return 0;
        }
    }
}