#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void
tree_travel(int fp1, int32_t offset)
{
    struct Node list;

    if (lseek(fp1, offset, SEEK_SET) == -1){
        return;
    }

    if (read(fp1, &list, sizeof(list)) != sizeof(list)) {
        return;
    }

    if (list.right_idx) {
        tree_travel(fp1, list.right_idx * sizeof(list));
    }

    printf("%d\n", list.key);

    if (list.left_idx) {
        tree_travel(fp1, list.left_idx * sizeof(list));
    }

}



int
main(int argc, char * argv[])
{
    int fp1 = open(argv[1], O_RDONLY);
    if (fp1 < 0) {
        return 0;
    }

    tree_travel(fp1, 0);

    return 0;
}