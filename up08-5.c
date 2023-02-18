#include <dirent.h>
#include <string.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
int
str_cmp(const void *s1, const void *s2)
{
    return strcasecmp(*(const char **)s1, *(const char **)s2);
}

void
recursive_travel(const char *dir)
{
    DIR *d = opendir(dir);
    if (!d) {
        return;
    }

    char **str = calloc(2, sizeof(*str));

    int size = 2;
    int cnt_of_dirs = 0;
    struct dirent *dd;
    char path[PATH_MAX];

    while ((dd = readdir(d))) {
        if (strcmp(dd->d_name, ".") && strcmp(dd->d_name, "..")) {
            int slen = snprintf(path, sizeof(path), "%s/%s", dir, dd->d_name);
            if (slen < sizeof(path)) {

                struct stat stb;
                if ((lstat(path, &stb) != -1) && (S_ISDIR(stb.st_mode))) {
                    if (cnt_of_dirs >= size) {
                        size <<= 1;
                        str = realloc(str, size * sizeof(*str));
                    }
                    
                    str[cnt_of_dirs++] = strdup(dd->d_name);
                }
            }
        }
    }
    closedir(d);

    qsort(str, cnt_of_dirs, sizeof(*str), str_cmp);

    for (int i = 0; i < cnt_of_dirs; i++) {
        snprintf(path, sizeof(path), "%s/%s", dir, str[i]);
        if ((d = opendir(path))) {
            closedir(d);
            printf("cd %s\n", str[i]);
            recursive_travel(path);
            printf("cd ..\n");
        }
        free(str[i]);
    }
    free(str);
}



int main(int argc, char *argv[])
{
    recursive_travel(argv[1]);

    return 0;
}