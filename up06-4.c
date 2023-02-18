#include <stdio.h>
#include <time.h>

enum
{
    YEAR = 1900,
    DAY = 366,
    CONDITION = 3,
    THURSDAY = 4
};

int
main(void)
{
    int year;
    scanf("%d", &year);

    struct tm tmm;

    tmm.tm_sec = 0;
    tmm.tm_min = 0;
    tmm.tm_hour = 0;
    tmm.tm_year = year - YEAR;

    int count = 0;
    int prev_mon = 0;
    for (int i = 1; i < DAY; i++) {
        tmm.tm_mday = i;
        tmm.tm_mon = 0;
        tmm.tm_isdst = -1;
        mktime(&tmm);

        if (tmm.tm_mon != prev_mon) {
            count = 0;
        }
        prev_mon = tmm.tm_mon;

        if (tmm.tm_wday == THURSDAY) {
            count++;
            if (!(count & 1) && (tmm.tm_mday % CONDITION)) {
                printf("%d %d\n", tmm.tm_mon + 1, tmm.tm_mday);
                count = 0;
            }
        }
    }

    return 0;
}