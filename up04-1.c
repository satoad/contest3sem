#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

enum 
{
    BUF = 251,
    DIGITS = 6
};

int
main(int argc, char const *argv[])
{
    int fp1 = open(argv[1], O_RDONLY);

    unsigned short num[BUF] = {0};
    int voice_sum[DIGITS] = {0};

    unsigned short previos_count;
    unsigned short current_count;

    int cnt1 = read(fp1, &previos_count, sizeof(previos_count));

    unsigned short current_sum;

    int cnt2 = read(fp1, &current_sum, sizeof(current_sum));

    while ((cnt1 == sizeof(previos_count) && (cnt2 == sizeof(current_sum)) && (previos_count != 0))) {

        int cnt3 = read(fp1, num, sizeof(num[0]) * previos_count);
        if (cnt3 != sizeof(num[0]) * previos_count) {
            break;
        }

        cnt1 = read(fp1, &current_count, sizeof(current_count));

        cnt2 = read(fp1, &current_sum, sizeof(current_sum));

        int tmp_sum = 0;

        for (int i = 0; i < previos_count; i++) {
            tmp_sum += num[i];
        }

        if (tmp_sum != current_sum) {
            for (int i = 0; i < previos_count; i++) {
                if (num[i] != 1) {
                    num[i]++;
                    tmp_sum++;
                }
                voice_sum[num[i] - 1]++;
            }
        } else {
            for (int i = 0; i < previos_count; i++) {
                voice_sum[num[i] - 1]++;
            }
        }

        voice_sum[1] += current_sum - tmp_sum;
        voice_sum[0] -= current_sum - tmp_sum;

        previos_count = current_count;

    }

    for (int i = 0; i < DIGITS; i++) {
        printf("%d\n", voice_sum[i]);
    }
    
    return 0;
}