#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

enum
{
    BUF_SIZE = 4098,
    DIGITS = 10
};

int
main(void)
{
    char file[BUF_SIZE] = {0};
    unsigned long long num[DIGITS] = {0};
    if (fgets(file, sizeof(file), stdin)) {

        int i = 0;
        while ((file[i] != '\r') && (file[i] != '\0') && (file[i] != '\n')) {
            i++;
        }
        
        file[i] = '\0';

        int fp1 = open(file, O_RDONLY, 0777);

        if (fp1 >= 0) {
            char content[BUF_SIZE] = {0};

            int cnt;
            while ((cnt = read(fp1, content, sizeof(content))) > 0) {
                for(i = 0; i < cnt; i++) {
                    if ((content[i] >= '0') && (content[i] <= '9')) {
                        num[content[i] - '0']++;
                    }
                }
            }

            close(fp1);
        }
    }


    for (int i = 0; i < DIGITS; i++) {
        printf("%d %llu\n", i, num[i]);
    }

    return 0;
}