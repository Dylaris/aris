#define ARIS_IMPLEMENTATION
#define ARIS_STRIP_PREFIX
#include "aris.h"

int main(void)
{
    Aris_Mini_Hash lookup = {0};
    const char *str;

    for (int i = 1; i < 100; i++) {
        str = string_tmp_format("item_%d", i);
        aris_hash_set(&lookup, fnv_hash(str, strlen(str)), i);
    }

    uint32_t x;

    for (int i = 1; i < 100; i++) {
        str = string_tmp_format("item_%d", i);
        if (!aris_hash_get(&lookup, fnv_hash(str, strlen(str)), &x)) return 1;
        assert(x == (uint32_t)i);
        printf("%s: %u\n", str, x);
    }

    aris_hash_free(&lookup);

    return 0;
}
