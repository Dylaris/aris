#define ARIS_IMPLEMENTATION
#include "aris.h"

int main(void)
{
    char **names;

    names = NULL;

    aris_vecpush(names, "Jack");
    aris_vecpush(names, "John");
    aris_vecpush(names, "Tom");
    aris_vecpush(names, "Ben");
    aris_vecpush(names, "Mike");

    printf("size: %zu\ncapacity: %zu\n", aris_vecsize(names), aris_veccap(names));

    for (size_t i = 0; i < aris_vecsize(names); i++) {
        printf("id: %zu, name: %s\n", i, names[i]);
    }

    aris_vecreset(names);

    aris_vecpush(names, "1");
    aris_vecpush(names, "11");
    aris_vecpush(names, "111");

    printf("---------- after reset ---------- \n");
    printf("size: %zu\ncapacity: %zu\n", aris_vecsize(names), aris_veccap(names));

    for (size_t i = 0; i < aris_vecsize(names); i++) {
        printf("id: %zu, name: %s\n", i, names[i]);
    }

    aris_vecfree(names);
    return 0;
}
