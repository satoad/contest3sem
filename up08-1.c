#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stddef.h>
#include <sys/stat.h>

enum
{
    FILE_SIZE = 65536,
    BASE = 16,
    PAGE_NUM_OFFSET = 9
};

int
main(int argc, char *argv[])
{
    int fp1 = open(argv[1], O_RDONLY);
    if (fp1 < 0) {
        return 1;
    }

    struct stat file;
    if (fstat(fp1, &file) < 0) {
        return 1;
    }

    if (file.st_size != FILE_SIZE) {
        return 1;
    }

    unsigned short * mmap_fp1 = mmap(NULL, FILE_SIZE, PROT_READ, MAP_PRIVATE, fp1, 0);
    if (mmap_fp1 == MAP_FAILED) {
        return 1;
    }
    close(fp1);

    unsigned table_offset = strtol(argv[2], NULL, BASE);
    unsigned short *table = mmap_fp1 + table_offset / 2;

    unsigned short offset;
    while(scanf("%hx", &offset) == 1) {
        unsigned short vpn = offset >> PAGE_NUM_OFFSET;
        unsigned short vpo = offset & 0x1FF;
        unsigned short paddr = table[vpn] + vpo;
        printf("%hu\n", mmap_fp1[paddr / 2]);
    }

    if (munmap(mmap_fp1, FILE_SIZE) < 0) {
        return 1;
    }
    
    return 0;
}