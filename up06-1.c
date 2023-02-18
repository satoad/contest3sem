#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

enum { PRESICION = 1000000 };

int 
main(int argc, char * argv[])
{
    struct timeval tv;
    tv.tv_sec = strtol(argv[1], 0, 10);
    tv.tv_usec = strtol(argv[2], 0, 10);

    double lyamd = strtod(argv[3], 0);
    int k = strtol(argv[4], 0, 10);
    unsigned int s = strtol(argv[5], 0, 10);

    srand(s); 

    for (int i = 0; i < k; i++) {
        double u = (double)rand()/RAND_MAX;

        long microsec = (long)(-log(u)/lyamd);
        tv.tv_usec += microsec;
        tv.tv_sec += tv.tv_usec / PRESICION;
        tv.tv_usec %= PRESICION;

        printf("%ld %ld\n", tv.tv_sec, tv.tv_usec);
    }

    return 0;
}