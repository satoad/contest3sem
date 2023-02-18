#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

int
main(int argc, char * argv[])
{
    int fp1 = open(argv[1], O_RDONLY);
    if (fp1 < 0) {
        return 1;
    }

    unsigned short int min = USHRT_MAX;
    int flag = 0;
    unsigned short value = 0;
    unsigned char num[sizeof(value)];
    while (read(fp1, &num, sizeof(num)) == sizeof(num)) {
        for (int i = 0; i < sizeof(value); i++) {
            value <<= CHAR_BIT;
            value |= num[i];
        }

        if ((value <= min) && (!(value & 1))) {
            min = value;
            flag = 1;
        }
    }

    if (flag) {
        printf("%d\n", min);
    }

    return 0;
}