#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/wait.h>

enum { RET = 42 };

int
main(int argc, char * argv[])
{   
    int pid;
    if ((pid = fork()) == 0) {
        int fp0 = open(argv[2], O_RDONLY);
        if (fp0 < 0) {
            _exit(RET);
        }
        if (dup2(fp0, 0) == -1) {
            _exit(RET);
        }
        close(fp0);

        int fp1 = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660);
        if (fp1 < 0) {
            _exit(RET);
        }

        if (dup2(fp1, 1) == -1) {
            _exit(RET);
        }
        close(fp1);

        int fp2 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if (fp2 < 0) {
            _exit(RET);
        }
        dup2(fp2, 2);
        close(fp2);

        if (execlp(argv[1], argv[1], NULL) == -1) {
            _exit(RET);
        }

    } else if (pid < 0) {
        exit(RET);
    } else {
        int status;
        wait(&status);
        printf("%d\n", status);
    }

    return 0;
}