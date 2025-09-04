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
        aris_todo("this is todo");
        break;

    case 2:
        aris_fatal("this is fatal");
        break;

    case 3:
        aris_error("this is error");
        break;

    default:
        aris_unreachable("this is unreachable");
        break;
    }

    return 0;
}
