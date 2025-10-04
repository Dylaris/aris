#define ARIS_IMPLEMENTATION
#include "aris.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: ./log [number]\n");
        return 1;
    }

    switch (atoi(argv[1])) {
    case 1:
        aris_log(ARIS_LOG_TODO, "this is todo");
        break;

    case 2:
        aris_log(ARIS_LOG_ERROR, "this is error");
        break;

    case 3:
        aris_log(ARIS_LOG_INFO, "this is info");
        break;

    case 4:
        aris_log(ARIS_LOG_WARN, "this is warning");
        break;
    }

    return 0;
}
