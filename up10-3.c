#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/wait.h>

int exec(const char *cmd) {
    int pid;
    if ((pid = fork()) == 0) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (pid < 0) {
        return 0;
    } else {
        int status;
        wait(&status);
        return WIFEXITED(status) && (WEXITSTATUS(status) == 0);
    }
}

int 
main(int agrc, char* argv[]) 
{
    return !((exec(argv[1]) || exec(argv[2])) && (exec(argv[3])));
}