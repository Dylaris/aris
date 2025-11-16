#define COOK_IMPLEMENTATION
#define COOK_STRIP_PREFIX
#include "cook.h"

size_t fnv_hash(const char *str, size_t length)
{
    size_t hash = 2166136261u;
    for (size_t i = 0; i < length; i++) {
        hash ^= (unsigned char)str[i];
        hash *= 16777619;
    }
    return hash;
}

int main(void)
{
    Cook_Mini_Hash lookup = {0};
    const char *str;

    for (int i = 1; i < 100; i++) {
        str = string_tmp_format("item_%d", i);
        cook_hash_set(&lookup, fnv_hash(str, strlen(str)), i);
    }

    size_t x;

    for (int i = 1; i < 100; i++) {
        str = string_tmp_format("item_%d", i);
        if (!cook_hash_get(&lookup, fnv_hash(str, strlen(str)), &x)) return 1;
        assert(x == (size_t)i);
        printf("%s: %zu\n", str, x);
    }

    cook_hash_free(&lookup);

    return 0;
}
