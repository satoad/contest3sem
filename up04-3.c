#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <limits.h>

enum
{
    BUF = 16, // length of comment
    KN_IN_GAL = 493,
    KN_IN_SIK = 29,
};

int
main(int argc, char *argv[])
{
    int value = 0;
    long long int max1 = INT64_MIN;
    long long int max2 = INT64_MIN;
    int flag = 0;

    for (int i = 1; i < argc; i++) {
        int fp1 = open(argv[i], O_RDONLY);
        if (fp1 < 0) {
            continue;
        }

        char s[BUF] = {0};
        int cnt1 =  read(fp1, s, sizeof(s));
        if (cnt1 != sizeof(s)) {
            continue;
        }

        while (cnt1 >= sizeof(s)) {
            unsigned char num[sizeof(value)];
            cnt1 = read(fp1, &num, sizeof(num));
            if (cnt1 != sizeof(num)) {
                break;
            }

            unsigned int tmp = 0;
            for (int i = 0; i < sizeof(value); i++) {
                tmp <<= CHAR_BIT;
                tmp |= num[i];
            }

            value = (int)tmp;

            if (value > max1) {
                max2 = max1;
                max1 = value;
                flag++;
            } else if ((value > max2) && (value != max1)) {
                max2 = value;
                flag++;
            }

            cnt1 =  read(fp1, s, sizeof(s));
            value = 0;
        }
    }

    if (flag < 2) {
        return 0;
    }

    if (max2 < 0) {
        max2 = -max2;
        printf("-");
    }

    long long int gal = max2 / KN_IN_GAL;
    max2 -= gal * KN_IN_GAL;

    long long int sik = max2 / KN_IN_SIK;
    max2 -= sik * KN_IN_SIK;

    printf("%lldg%.2llds%.2lldk\n", gal, sik, max2);

    return 0;
