#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <dirent.h>

int 
main(int argc, char * argv[])
{
    DIR *d = opendir(argv[1]);

    struct dirent *dd;
    char path[PATH_MAX];
    unsigned long long count = 0;
    while ((dd = readdir(d))) {
        int slen = snprintf(path, sizeof(path), "%s/%s", argv[1], dd->d_name);
        if (slen < sizeof(path)) {
            char sample[] = ".exe";
            int len = sizeof(sample) - 1;
            int flag = 0;
            for (int i = 0; i < len; i++) {
                if (path[slen - len + i] != sample[i]) {
                    flag = 1;
                } 
            }

            struct stat stat_fp1;
            if ((!stat(path, &stat_fp1)) && (!flag) && (S_ISREG(stat_fp1.st_mode)) && (!access(path, X_OK))) {
                count++;
            }
        }
    }
    printf("%llu\n", count);
    return 0;
}