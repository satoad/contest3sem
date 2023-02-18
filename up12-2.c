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

enum
{
    MODE_ADD = 0,
    MODE_MUL
};

volatile int mode = 0;

void
handler(int signal) {
    if (signal == SIGINT) {
        mode = MODE_ADD;
    } else if (signal == SIGQUIT) {
        mode = MODE_MUL;
    }
}

int
main(void)
{
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGQUIT, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    int pid = getpid();
    printf("%d\n", pid);
    fflush(stdout);

    unsigned sum = 0;
    int num;
    while (scanf("%d", &num) == 1) {
        if (!mode) {
            sum += num;
        } else if (mode) {
            sum *= num;
        }

        printf("%d\n", sum);
        fflush(stdout);
    }

    return 0;
