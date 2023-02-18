#include <stdio.h>
#include <stdlib.h>

const char rool[] = "rwxrwxrwx";

enum { LENGTH = sizeof(rool) - 1 };

int
main(int argc, char * argv[])
{
    for (int i = 1; i < argc; i++) {
        long int num = strtol(argv[i], 0, LENGTH - 1);
        for (int i = 0; i < LENGTH; i++) {
            int tmp = num >> (LENGTH - 1 - i);

            if (!(tmp & 1)) {
                putchar('-');
            } else {
                putchar(rool[i]);
            }
        }

        putchar('\n');
    }
    return 0;
}