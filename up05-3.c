#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

enum { MAXN = 32 };

int
main(void)
{
    uint32_t n;
    int32_t w;
    uint32_t s;
    scanf("%"PRIu32 "%"PRIu32 "%"PRIu32, &n, &s, &w);
    uint32_t max;
    if (n == MAXN) {
        max = 0xffffffff;
    } else {
        max = (1 << n) - 1;
    }

    for (uint32_t i = 0; i <= max; i += s) {
        int32_t num = i;
        if (i >> (n - 1)) {
            num = i - (max >> 1) - 1;
            num = -num;
        }
        printf("|%*"PRIo32"|%*"PRIu32"|%*"PRId32"|\n", (int) w, i, (int) w, i, (int) w, num);

        if (max - s < i) {
            break;
        }
    }
    return 0;
}