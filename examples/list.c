#define ARIS_IMPLEMENTATION
#define ARIS_STRIP_PREFIX
#include "aris.h"

typedef struct fruit {
    const char *name;
    INSTRUSIVE_LIST_NODE;
} fruit;

int main(void)
{
    fruit fruits[] = {
        LIST_ITEM_INITIALIZER(fruit, .name = "apple"),
        LIST_ITEM_INITIALIZER(fruit, .name = "orange"),
        LIST_ITEM_INITIALIZER(fruit, .name = "grape"),
        LIST_ITEM_INITIALIZER(fruit, .name = "banana")
    };

    list_init(fruit_list);

    for (size_t i = 0; i < sizeof(fruits)/sizeof(fruits[0]); i++) {
        list_add_tail(&fruit_list, &fruits[i]);
    }
    printf("size: %zu\nisempty: %s\n", list_size(&fruit_list), list_isempty(&fruit_list) ? "yes" : "no");

    list_foreach(fruit, &fruit_list, iter) {
        printf("%s ", iter->name);
    }
    printf("\n");

    return 0;
}
