#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int
main(void)
{
    int n;
    scanf("%d", &n);

    printf("1");
    for (int i = 2; i <= n; i++) {
        printf(" ");
        if (fork() == 0) {
            printf("%d", i);
            fflush(stdout);
        } else {
            int status;
            wait(&status);
            _exit(0);
        }
    }
    int status;
    wait(&status);
    printf("\n");
    return 0;
}