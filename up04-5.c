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
    int fp1 = open(argv[1], O_RDWR);
    if (fp1 < 0) {
        return 1;
    }

    long long int num;
    long long int min_num = INT64_MAX;
    long long int cur_offset = -1;

    int cnt1; 
    int flag = 0;
    long long int prev_offset = 0;
    while((cnt1 = read(fp1, &num, sizeof(num))) && (cnt1 == sizeof(num))) {
        if ((num < min_num) || ((num == min_num) && (!flag)))  {
            min_num = num;
            cur_offset = prev_offset;
            flag++;
        }
        prev_offset++;
    }

    if ((min_num == INT64_MIN) || (cur_offset == -1)) {
        return 0;
    }

    min_num = -min_num;

    cnt1 = lseek(fp1, cur_offset * sizeof(num), SEEK_SET);
    if (cnt1 == -1) {
        return 1;
    }
    
    cnt1 = write(fp1, &min_num, sizeof(min_num));
    if (cnt1 != sizeof(min_num)) {
            return 1;
    }

    return 0;
}