#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

int
main(void)
{
    int fd1[2];
    pipe(fd1);
    int pid1;
    if ((pid1 = fork()) == 0) {
        int pid2;
        if ((pid2 = fork()) == 0) {

            long long int x = 0;
            int tmp1 = 0;
            close(fd1[1]);
            while (read(fd1[0], &tmp1, sizeof(tmp1)) == sizeof(tmp1)) {
                x += tmp1;
            }

            close(fd1[0]);
            printf("%lld\n", x);
            fflush(stdout);
            _exit(0);

        } else if (pid2 < 0) {
            _exit(1);
        } else {
            close(fd1[0]);
            close(fd1[1]);

            wait(NULL);
            _exit(0);
        }
    } else if (pid1 < 0) {
        _exit(1);
    } else {

        int tmp;
        close(fd1[0]);
        while(scanf("%d", &tmp) == 1) {
            write(fd1[1], &tmp, sizeof(tmp));
        } 
        close(fd1[1]);
        wait(NULL);
    }
    return 0;
}