#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum { PRECISION = 10000 };

int main(int argc, char** argv){
    double value = atof(argv[1]);

    for (int i = 2; i < argc; i++){
        value *= (100.0 + atof(argv[i]))/100.0;
        value = round(value * PRECISION) / PRECISION;
    }

    printf("%.4lf\n", value);
    return 0;

}