#define COOK_IMPLEMENTATION
#include "cook.h"

int main(void)
{
    // 1. String formatting
    char *formatted = cook_string_tmp_format("User: %s, ID: %d", "Alice", 123);
    printf("Formatted: %s\n", formatted);

    // 2. In-place string reversal
    char str[] = "Hello";
    printf("Original: %s\n", str);
    cook_string_reverse(str);
    printf("Reversed: %s\n", str);

    // 3. Create reversed copy
    const char *original = "ABCD";
    char *reversed = cook_string_tmp_reverse(original);
    printf("Original: %s → Reversed: %s\n", original, reversed);

    // 4. Prefix check
    printf("Has prefix 'Hello': %d\n", cook_string_has_prefix("Hello world", "Hello"));

    // 5. Postfix check
    printf("Has postfix 'world': %d\n", cook_string_has_postfix("Hello world", "world"));

    // 6. Trim whitespace
    char s[] = "  \thello world\n\t\t  ";
    printf("Trim whitespace: '%s'\n", cook_string_tmp_trim(s));
    cook_string_trim(s);
    printf("Trim whitespace: '%s'\n", s);

    return 0;
}
