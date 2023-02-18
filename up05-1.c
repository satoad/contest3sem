#include <stdio.h>
#include <sys/stat.h>

enum { KIBIBYTE = 1024 };

int
main(int argc, char * argv[])
{
    struct stat stat_fp1;

    long long int sum = 0;
    for (int i = 1; i < argc; i++) {
        if ((lstat(argv[i], &stat_fp1) >= 0) && (S_ISREG(stat_fp1.st_mode)) 
                && (stat_fp1.st_size % KIBIBYTE == 0) && (stat_fp1.st_nlink == 1)) {
            sum += stat_fp1.st_size;
        }
    }

    printf("%lld\n", sum);
    return 0;
}