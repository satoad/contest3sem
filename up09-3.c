#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

enum 
{
    INPUT_NUM = 3, 
    STR_LENGTH = 8,
    BASE = 10
};

int 
main(void)
{
    for (int i = 1; i <= INPUT_NUM; i++) {
        if (fork() == 0) {
            char input[STR_LENGTH] = {0};
            read(0, input, sizeof(input));
            long int num = strtol(input, NULL, BASE);
            num *= num;
            printf("%d %ld\n", i, num);
            fflush(stdout);
            _exit(0);
        } else {
            wait(NULL);
        }
    }
    return 0;
}

Нет гарантии, что \0 находится в буфере, передаваемом в strtol
Процессы не выполняются параллельно