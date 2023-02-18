#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/wait.h>
#include <string.h>

int
main(int argc, char* argv[])
{
    char *dir_path = getenv("XDG_RUNTIME_DIR");
    if (!dir_path) {
        dir_path = getenv("TMPDIR");
        if (!dir_path) {
            dir_path = "/tmp";
        }
    }

    unsigned rand_num;
    int fp1 = open("/dev/urandom", O_RDONLY);
    if (fp1 < 0) {
        exit(1);
    }
    read(fp1, &rand_num, sizeof(rand_num));
    close(fp1);

    char full_path[PATH_MAX];
    int slen = snprintf(full_path, PATH_MAX, "%s/%u%s", dir_path, rand_num, ".py");
    if (slen <= sizeof(full_path)) {
        int fp2 = open(full_path,  O_WRONLY | O_CREAT | O_TRUNC, 0700);
        if (fp2 < 0) {
            return 1;
        }

        char *text = "#!/usr/bin/env python3\nimport os\nprint(";
        if (write(fp2, text, strlen(text)) == strlen(text)) {
            for (int i = 1; i < argc; i++) {
                if (write(fp2, argv[i], strlen(argv[i])) == strlen(argv[i])) {
                    if (i != argc - 1) {
                        if (write(fp2, "*", 1) != 1) {
                            return 1;
                        }
                    }
                }
            }
        }
        
        char *remove = ")\nos.remove(__file__)";
        if (write(fp2, remove, strlen(remove)) == strlen(remove)) {
            close(fp2);

            if (execl(full_path, full_path, NULL) == -1) {
                return 1;
            }
        }
    }

    return 0;
}