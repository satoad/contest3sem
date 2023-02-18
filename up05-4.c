#include <stdio.h>

const char sample[] = "rwxrwxrwx";

enum { LENGTH = sizeof(sample) - 1 };

int
parse_rwx_permissions(const char *str)
{
    if (!str) {
        return -1;
    }

    int rool = 0;
    for (int i = 0; i < LENGTH; i++) {
        if (str[i] == sample[i]) {
            rool <<= 1;
            rool |= 1;
        } else if (str[i] == '-') {
            rool <<= 1;
        } else {
            return -1;
        }

    }

    if (str[LENGTH]) {
        return -1;
    }

    return rool;
}