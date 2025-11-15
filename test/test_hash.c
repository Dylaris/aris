#define COOK_IMPLEMENTATION
#define COOK_STRIP_PREFIX
#include "cook.h"

int main(void)
{
    Cook_Mini_Hash lookup = {0};
    const char *str;

    for (int i = 1; i < 100; i++) {
        str = string_tmp_format("item_%d", i);
        cook_hash_set(&lookup, fnv_hash(str, strlen(str)), i);
    }

    uint32_t x;

    for (int i = 1; i < 100; i++) {
        str = string_tmp_format("item_%d", i);
        if (!cook_hash_get(&lookup, fnv_hash(str, strlen(str)), &x)) return 1;
        assert(x == (uint32_t)i);
        printf("%s: %u\n", str, x);
    }

    cook_hash_free(&lookup);

    return 0;
}
