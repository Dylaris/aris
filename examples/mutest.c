#define COOK_IMPLEMENTATION
#include "cook.h"

int main(void) {
    COOK_MUTEST(cook_expect_str_eq("hell", "hello"), "string should be equal");
    COOK_MUTEST(1 == 1, "number should be equal");
    cook_mutest_detail();
    cook_mutest_summary();
    return 0;
}
