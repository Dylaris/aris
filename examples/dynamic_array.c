#define COOK_IMPLEMENTATION
#include "cook.h"

typedef struct name_array {
    char **items;
    size_t len;
    size_t cap;
} names_t;

int main(void)
{
    names_t names = {0};

    cook_vec_push(&names, "Jack");
    cook_vec_push(&names, "John");
    cook_vec_push(&names, "Tom");
    cook_vec_push(&names, "Ben");
    cook_vec_push(&names, "Mike");

    printf("len: %zu\ncap: %zu\n", names.len, names.cap);

    cook_vec_foreach(char*, &names, it) {
        printf("id: %zu, name: %s\n", it-names.items, *it);
    }

    cook_vec_reset(&names);

    cook_vec_push(&names, "1");
    cook_vec_push(&names, "11");
    cook_vec_push(&names, "111");

    printf("---------- after reset ---------- \n");
    printf("len: %zu\ncap: %zu\n", names.len, names.cap);

    cook_vec_foreach(char*, &names, it) {
        printf("id: %zu, name: %s\n", it-names.items, *it);
    }

    cook_vec_free(&names);
    return 0;
}
