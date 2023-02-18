#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

enum 
{
    YEAR = 1900,
    PARAMETERS = 3
};

int 
main(void)
{
    int fd1[2];
    pipe(fd1);

    int pid1;
    if ((pid1 = fork()) == 0) {
        int pid2;
        if ((pid2 = fork()) == 0) {
            int pid3;
            if ((pid3 = fork()) == 0) {
                time_t cur = time(NULL);

                close(fd1[0]);
                for (int i = 0; i < PARAMETERS; i++) {
                    if (write(fd1[1], &cur, sizeof(cur)) != sizeof(cur)) {
                        _exit(1);
                    }
                }

                close(fd1[1]);
                _exit(0);

            } else if (pid3 < 0) {
                close(fd1[0]);
                close(fd1[1]);

                _exit(1);
            } else {
                int status;
                waitpid(pid3, &status, 0);
                
                time_t day;
                if (read(fd1[0], &day, sizeof(day)) == sizeof(day)) {
                    struct tm ttm;
                    localtime_r(&day, &ttm);

                    printf("D:%02d\n", ttm.tm_mday);
                    fflush(stdout);

                    close(fd1[0]);
                    close(fd1[1]);

                    _exit(0);
                } else {
                    close(fd1[0]);
                    close(fd1[1]);

                    _exit(1);
                }
            }
        } else if (pid2 < 0) {
            close(fd1[0]);
            close(fd1[1]);

            _exit(1);
        } else {
            int status;
            waitpid(pid2, &status, 0);

            time_t month;
            if (read(fd1[0], &month, sizeof(month)) == sizeof(month)) {
                struct tm ttm;
                localtime_r(&month, &ttm);

                printf("M:%02d\n", ttm.tm_mon + 1);
                fflush(stdout);

                close(fd1[0]);
                close(fd1[1]);

                _exit(0);
            } else {
                close(fd1[0]);
                close(fd1[1]);
                    
                _exit(1);
            }
        }
    } else if (pid1 < 0) {
        close(fd1[0]);
        close(fd1[1]);

        exit(1);
    } else {
        int status;
        waitpid(pid1, &status, 0);
        time_t year;
        if (read(fd1[0], &year, sizeof(year)) == sizeof(year)) {
            struct tm ttm;
            localtime_r(&year, &ttm);

            printf("Y:%04d\n", ttm.tm_year + YEAR);
            fflush(stdout);

            close(fd1[0]);
            close(fd1[1]);

            exit(0);
        } else {
            close(fd1[0]);
            close(fd1[1]);
                    
            exit(1);
        }
    }

    return 0;
}