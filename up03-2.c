#include <stdlib.h>
#include <stdio.h>

enum { BASIS = 10 };

int main(int argc, char ** argv) {
    long long pos_sum = 0, neg_sum = 0;
    for (int i = 1; i < argc; i++) {
        int value = strtol(argv[i], 0, BASIS);

        if (value > 0) {
            pos_sum += value;
        } else {
            neg_sum += value;
        }
    }

    printf("%lld\n%lld\n", pos_sum, neg_sum);
    return 0;
}