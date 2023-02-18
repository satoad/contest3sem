#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/wait.h>

int mysys(const char *str) {
    int pid;
    if ((pid = fork()) == 0) {
        execlp("/bin/sh", "sh", "-c", str, NULL);
        _exit(127);
    } else if (pid < 0) {
        return -1;
    } else {
        int status;
        waitpid(pid,&status, 0);
        if (WIFSIGNALED(status)) {
            return WTERMSIG(status) + 128;
        } else if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
    }
    return 0;
}