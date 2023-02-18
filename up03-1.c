#include <limits.h>

STYPE bit_reverse(STYPE value) {
    UTYPE num = 0;
    UTYPE value1 = (UTYPE)value;

    int size = CHAR_BIT * sizeof(UTYPE);

    while (size) {
        int tmp = value1 & 1;
        num <<= 1;
        num ^= tmp;
        value1 >>= 1;
        size--;
    }

    return num;
}