#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int 
main(void)
{
    if (fork() == 0) {
        if (fork() == 0) {
            printf("3 ");
            fflush(stdout);
            _exit(0);
        } else {
            int status;
            wait(&status);
            printf("2 ");
            fflush(stdout);
            _exit(0);
        }
    } else {
        int status;
        wait(&status);
        printf("1\n");
    }
    
    return 0;
}