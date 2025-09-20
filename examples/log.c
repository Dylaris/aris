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
        ARIS_TODO("this is todo");
        break;

    case 2:
        ARIS_FATAL("this is fatal");
        break;

    case 3:
        ARIS_ERROR("this is error");
        break;

    default:
        ARIS_UNREACHABLE("this is unreachable");
        break;
    }

    return 0;
}
