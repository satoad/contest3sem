#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

enum { ROOL = 0600 };

int 
main(int argc, char * argv[])
{
    int fp1 = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, ROOL);

    unsigned int little_num;

    while(scanf("%u", &little_num) == 1){

        unsigned fpart = (little_num << 4) & 0x0fff0000;
        little_num &= 0xfff;
        little_num |= fpart;

        unsigned big_num = 0;
        big_num |= ((0x000000ff & little_num) << 24);
        big_num |= ((0x0000ff00 & little_num) << 8);
        big_num |= ((0x00ff0000 & little_num) >> 8);
        big_num |= ((0xff000000 & little_num) >> 24);

        int cnt = write(fp1, &big_num, sizeof(big_num));
        if (cnt != sizeof(big_num)) {
            return 0;
        }
    }

    return 0;
}