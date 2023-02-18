#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

int 
main(int argc, char * argv[]) 
{
    long long result = 0;
    for (int i = 1; i < argc; i++) {
        int len = strlen(argv[i]);

        if (len < 2) {
            continue;
        }

    
        if ((argv[i][len -2] < '0' && (argv[i][len - 2] > '9')) && (argv[i][len - 2] != 'k')) {
            continue;
        }

        if ((argv[i][len - 1] != '-') && (argv[i][len -1] != '+')) {
            continue;
        }

        int flag2 = 0;
        if (argv[i][len - 2] == 'k') {
            len -= 2;
            flag2 = 1;
        }

        argv[i][len] = '\0';
        long long n = atoi(argv[i]);
       
        if (flag2) {
            n *= 1000;
        }

        if ((n < LONG_MIN) || (n > LONG_MAX)) {
            continue;
        }

        if (argv[i][len - 1] == '-') {
            result -= n;  
        } else {
            result += n;
        }
    
    }
    printf("%lld\n", result);
    return 0;
}