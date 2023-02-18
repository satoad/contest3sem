#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

int main(void)
{
    int n;
    int pid_start = getpid();
    while (scanf("%d", &n) == 1) {
        int pid;
        if ((pid = fork()) < 0) {
            _exit(1);
        } else if (!pid) {
            continue;
        } else {
            int status;
            waitpid(pid, &status, 0);
            int pid = getpid();
            if (WIFEXITED(status) && WEXITSTATUS(status)){
                if (pid == pid_start) {
                    printf("-1\n");
                    fflush(stdout);
                    exit(0);
                }
                exit(1);
            }

            printf("%d\n", n);
            fflush(stdout);
            
            exit(0);
        }
    }

    return 0;
}