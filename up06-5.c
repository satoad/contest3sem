#include <sys/stat.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

enum
{
    OWNER = 6,
    GROUP = 3
};

int
rool_check(const struct stat *stb, int access, int class, int class_offset) {
    int rool = (stb->st_mode & class_offset) >> class;
    return ((rool & access) == access);
}   

int
myaccess(const struct stat *stb, const struct Task *task, int access) {
    if (!(task->uid)) {
        return 1;
    }

    if (task->uid == stb->st_uid) {
        return rool_check(stb, access, OWNER, 0700);
    }


    for (int i = 0; i < task->gid_count; i++) {
        if (stb->st_gid == task->gids[i]) {
            return rool_check(stb, access, GROUP, 0070);
        }
    }

    return rool_check(stb, access, 0, 7);
}