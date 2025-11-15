#define COOK_IMPLEMENTATION
#include "cook.h"
#include <stdio.h>

int main(void)
{
    char **names = NULL;

    cook_vec_push(names, "Jack");
    cook_vec_push(names, "John");
    cook_vec_push(names, "Tom");
    cook_vec_push(names, "Ben");
    cook_vec_push(names, "Mike");

    printf("size: %zu\ncapacity: %zu\n", cook_vec_size(names), cook_vec_capacity(names));

    cook_vec_foreach(char*, names, it) {
        printf("id: %zu, name: %s\n", it-names, *it);
    }

    cook_vec_reset(names);

    cook_vec_push(names, "1");
    cook_vec_push(names, "11");
    cook_vec_push(names, "111");

    printf("---------- after reset ---------- \n");
    printf("size: %zu\ncapacity: %zu\n", cook_vec_size(names), cook_vec_capacity(names));

    cook_vec_foreach(char*, names, it) {
        printf("id: %zu, name: %s\n", it-names, *it);
    }

    cook_vec_free(names);
    return 0;
}
