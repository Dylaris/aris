#define ARIS_IMPLEMENTATION
#include "aris.h"

int main(void)
{
    char **names = NULL;

    aris_vec_push(names, "Jack");
    aris_vec_push(names, "John");
    aris_vec_push(names, "Tom");
    aris_vec_push(names, "Ben");
    aris_vec_push(names, "Mike");

    printf("size: %zu\ncapacity: %zu\n", aris_vec_size(names), aris_vec_capacity(names));

    for (size_t i = 0; i < aris_vec_size(names); i++) {
        printf("id: %zu, name: %s\n", i, names[i]);
    }

    aris_vec_reset(names);

    aris_vec_push(names, "1");
    aris_vec_push(names, "11");
    aris_vec_push(names, "111");

    printf("---------- after reset ---------- \n");
    printf("size: %zu\ncapacity: %zu\n", aris_vec_size(names), aris_vec_capacity(names));

    for (size_t i = 0; i < aris_vec_size(names); i++) {
        printf("id: %zu, name: %s\n", i, names[i]);
    }

    aris_vec_free(names);
    return 0;
}
