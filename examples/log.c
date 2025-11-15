#define COOK_IMPLEMENTATION
#include "cook.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: ./log [number]\n");
        return 1;
    }

    switch (atoi(argv[1])) {
    case 1:
        cook_log(COOK_LOG_TODO, "this is todo");
        break;

    case 2:
        cook_log(COOK_LOG_ERROR, "this is error");
        break;

    case 3:
        cook_log(COOK_LOG_INFO, "this is info");
        break;

    case 4:
        cook_log(COOK_LOG_WARN, "this is warning");
        break;
    }

    return 0;
}
