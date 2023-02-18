#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <errno.h>

enum { BASE = 10 };

void
son_work(FILE* fp1, FILE* fp2, int son_num, long long int max)
{
    int val;
    while ((fscanf(fp1, "%d", &val) == 1) && (val + 1 < max)) {
        val++;
        printf("%d %d\n", son_num, val);
        fflush(stdout);
        fprintf(fp2, "%d ", val);
        fflush(fp2);
    }

    fclose(fp1);
    fclose(fp2);
}


int
main(int argc, char *argv[])
{
    long long int max = strtoll(argv[1], NULL, BASE);
    int fd1[2];
    pipe(fd1);

    int fd2[2];
    pipe(fd2);

    FILE* fp10 = fdopen(fd1[0], "r");
    FILE* fp21 = fdopen(fd2[1], "w");

    int pid1;
    if ((pid1 = fork()) == 0) {
        close(fd1[1]);
        close(fd2[0]);

        son_work(fp10, fp21, 1, max);

        _exit(0);
    } else if (pid1 < 0) {
        fclose(fp10);
        fclose(fp21);

        close(fd1[1]);
        close(fd2[0]);

        exit(1);
    } else {
        FILE* fp11 = fdopen(fd1[1], "w");
        FILE* fp20 = fdopen(fd2[0], "r");

        int pid2;
        if ((pid2 = fork()) == 0) {
            close(fd1[0]);
            close(fd2[1]);

            fprintf(fp11, "0\n");
            fflush(fp11);

            son_work(fp20, fp11, 2, max);

            _exit(0);
        } else if (pid2 < 0) {
            fclose(fp11);
            fclose(fp20);

            close(fd1[0]);
            close(fd2[1]);

            exit(1);
        } else {
            fclose(fp10);
            fclose(fp11);
            fclose(fp20);
            fclose(fp21);

            while(wait(NULL) > 0);

            printf("Done\n");
            fflush(stdout);

            exit(0);
        }
    }

    return 0;
}