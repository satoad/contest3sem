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

volatile int count = 0;

void 
handler(int signal)
{
    if (count != 5) {
        printf("%d\n", count);
        fflush(stdout);
    } 
    count++;
}


int 
main(void)
{   
    sigaction(SIGHUP, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    int pid = getpid();
    printf("%d\n", pid);
    fflush(stdout);

    while(count < 6) {
        pause();
    }

    return 0;
}