#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    int fd1[2];
    pipe(fd1);

    int pid1;
    if ((pid1 = fork()) == 0) {
        close(fd1[0]);

        int pid2;
        if ((pid2 = fork()) == 0) {
            int fp1 = open(argv[4], O_RDONLY);
            dup2(fp1, 0);
            close(fp1);

            dup2(fd1[1], 1);
            close(fd1[1]);

            execlp(argv[1], argv[1], NULL);
            _exit(1);
        } else if (pid2 < 0) {
            close(fd1[1]);
            _exit(1);
        } else {
            int status;
            waitpid(pid2, &status, 0);

            if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                int pid3;
                if ((pid3 = fork()) == 0) {
                    dup2(fd1[1], 1);
                    close(fd1[1]);
                    
                    execlp(argv[2], argv[2], NULL);

                    _exit(1);
                } else if (pid3 < 0) {
                    close(fd1[1]);

                    _exit(1);
                } else {
                    waitpid(pid3, &status, 0);
                    close(fd1[1]);
                    _exit(0);
                }    
            } else {
                close(fd1[1]);
                _exit(0);
            }
        }
    } else if (pid1 < 0) {
        close(fd1[0]);
        close(fd1[1]);
        exit(1);
    } else {
        int pid4;
        if ((pid4 = fork()) == 0) {
            dup2(fd1[0], 0);
            close(fd1[0]);

            int fp2 = open(argv[5], O_WRONLY | O_CREAT | O_APPEND, 0666);
            dup2(fp2, 1);
            close(fp2);
            close(fd1[1]);

            execlp(argv[3], argv[3], NULL);
            _exit(1);
        } else if (pid4 < 0) {
            exit(1);
        } else {
            close(fd1[0]);
            close(fd1[1]);
            
            int status;
            waitpid(pid1, &status, 0);
            waitpid(pid4, &status, 0);
            exit(0);
        }
    }
}