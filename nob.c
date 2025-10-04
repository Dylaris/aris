#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

#define EXAMPLE_FOLDER "examples/"

static const char *example_src[] = {
    EXAMPLE_FOLDER"deque.c",
    EXAMPLE_FOLDER"dynamic_array.c",
    EXAMPLE_FOLDER"list.c",
    EXAMPLE_FOLDER"log.c",
    EXAMPLE_FOLDER"read_file.c",
    EXAMPLE_FOLDER"string_ops.c",
    EXAMPLE_FOLDER"mini_hash.c"
};

static const char *example_exe[] = {
    EXAMPLE_FOLDER"deque",
    EXAMPLE_FOLDER"dynamic_array",
    EXAMPLE_FOLDER"list",
    EXAMPLE_FOLDER"log",
    EXAMPLE_FOLDER"read_file",
    EXAMPLE_FOLDER"string_ops",
    EXAMPLE_FOLDER"mini_hash"
};

bool clear(void)
{
    for (size_t i = 0; i < ARRAY_LEN(example_exe); i++) {
        if (!nob_delete_file(example_exe[i])) return false;
    }
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (argc > 1 && strcmp(argv[1], "clear") == 0) {
        return clear() == true;
    }

    for (size_t i = 0; i < ARRAY_LEN(example_src); i++) {
        Cmd cmd = {0};
        cmd_append(&cmd, "clang");
        cmd_append(&cmd, "-Wall", "-Wextra");
        cmd_append(&cmd, "-std=c99");
        cmd_append(&cmd, "-I./");
        cmd_append(&cmd, "-o", example_exe[i], example_src[i]);
        if (!cmd_run(&cmd)) return 1;
    }

    return 0;
}
