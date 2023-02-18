#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stddef.h>

int
main(int argc, char* argv[])
{
    int rows = strtol(argv[2], NULL, 10);
    int cols = strtol(argv[3], NULL, 10);

    int fp1 = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fp1 < 0) {
        return 1;
    }

    int file_size;
    if (__builtin_smul_overflow(rows, cols, &file_size) != 0) {
        return 1;
    }

    if (__builtin_smul_overflow(file_size, sizeof(rows), &file_size) != 0) {
        return 1;
    }
    
    int pg_size = getpagesize();
    int mmap_size = (file_size / pg_size + 1) * pg_size; 


    ftruncate(fp1, file_size);
    int *mmap_fp1 = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, fp1, 0);
    if (mmap_fp1 == MAP_FAILED) {
        return 1;
    }
    close(fp1);

    long long int edge_offset = 0;
    unsigned num = 1;
    while ((edge_offset < (cols + 1)/2) && (edge_offset < (rows + 1)/ 2)) {
        long long int i = edge_offset;
        long long int j = edge_offset;
        for (; j < cols - edge_offset; j++) {
            if (mmap_fp1[i * cols + j] == 0) {
                mmap_fp1[i * cols + j] = num++;
            }
        }

        for (++i, --j; i < rows - edge_offset; i++) {
            if (mmap_fp1[i * cols + j] == 0) {
                mmap_fp1[j + i * cols] = num++;
            }
        }

        for (j--, i--; j >= edge_offset; j--) {
            if (mmap_fp1[i * cols + j] == 0) {
                mmap_fp1[i * cols + j] = num++;
            }
        }

        for (i--, j = edge_offset; i > edge_offset; i--) {
            if (mmap_fp1[i * cols + j] == 0) {
                mmap_fp1[i * cols + j] = num++;
            }
        }

        edge_offset++;
    }

    if (munmap(mmap_fp1, mmap_size) < 0) {
        return 1;
    }
    
    return 0;
}