#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <limits.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <math.h>

enum { BASE = 10 };

int
main(int argc, char *argv[])
{
    char *dir_path = getenv("XDG_RUNTIME_DIR");
    if (!dir_path) {
        dir_path = getenv("TMPDIR");
        if (!dir_path) {
            dir_path = "/tmp";
        }
    }

    char full_path1[PATH_MAX];
    int slen = snprintf(full_path1, PATH_MAX, "%s/%s", dir_path, "function.c");
    if (slen <= sizeof(full_path1)) {
        int fp1 = open(full_path1,  O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (fp1 < 0) {
            return 1;
        }

        char *text = "#include <math.h>\n double func(double x) {\n return ";
        if (write(fp1, text, strlen(text)) == strlen(text)) {
            if (write(fp1, argv[4], strlen(argv[4]))) {
                char *end = ";\n }";
                if (write(fp1, end, strlen(end))) {
                    close(fp1);

                    char full_path2[PATH_MAX];
                    slen = snprintf(full_path2, PATH_MAX, "%s/%s", dir_path, "function.o");
                    if (slen <= sizeof(full_path2)) {
                        int pid;
                        if ((pid = fork()) == 0) {
                            execlp("gcc", "gcc", "-fPIC", "-DPIC", "-shared", full_path1, 
                                "-o", full_path2, "-lm", NULL);
                            _exit(1);
                        } else if (pid < 0) {

                        } else {
                            int status;
                            waitpid(pid, &status, 0);

                            void *handle = dlopen(full_path2, RTLD_LAZY);
                            double (*func)(double) = (double (*)(double))dlsym(handle, "func");

                            double left = strtod(argv[1], NULL);
                            double right = strtod(argv[2], NULL);
                            long long int n = strtoll(argv[3], NULL, BASE);
                            double ans = 0.0;
                            double dx = (right - left) / n;

                            for (int i = 0; i < n; i++) {
                                double x = left + i * dx;
                                ans += func(x) * dx;
                            }

                            printf("%.10g\n", ans);
                            fflush(stdout);

                            dlclose(handle);
                            unlink(full_path1);
                            unlink(full_path2);
                        }
                    }
                }
            }
        }
    }
    return 0;
}