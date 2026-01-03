#define COOK_IMPLEMENTATION
#include "cook.h"

int main(void) {
    char *buffer = cook_fs_readfile(__FILE__);
    printf("%s", buffer);
    free(buffer);

    char *cwd;

    cwd = cook_fs_cwd();
    printf("cwd: %s\n", cwd);
    free(cwd);

    if (!cook_fs_mkdir("tmp/")) return 1;
    if (!cook_fs_move("tmp/", "new_tmp/")) return 1;
    if (!cook_fs_isdir("new_tmp/")) return 1;
    if (!cook_fs_rmdir("new_tmp/")) return 1;

    cook_attr_t attr;
    if (!cook_fs_stat(__FILE__, &attr)) return 1;
    printf("type: %s\n", cook_fs_type2string(&attr));
    printf("perm: %s\n", cook_fs_perm2string(&attr));
    printf("ino: %lld\n", attr.ino);
    printf("size: %lld\n", attr.size);
    printf("atime: %lld\n", attr.atime);
    printf("mtime: %lld\n", attr.mtime);
    printf("ctime: %lld\n", attr.ctime);

    cook_fs_chdir("..");
    cwd = cook_fs_cwd();
    printf("cwd: %s\n", cwd);
    free(cwd);

    cook_dir_t *dir = cook_fs_opendir(".");
    const char *filename;
    while ((filename = cook_fs_readdir(dir))) {
        printf("filename: %s\n", filename);
    }

    cook_fs_closedir(dir);

    return 0;
}
