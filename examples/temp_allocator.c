#define COOK_IMPLEMENTATION
#define COOK_STRIP_PREFIX
#include "cook.h"

int main(void)
{
    cook_temp_scope(1) {
        cook_temp_scope(2) {
            void *start1 = cook_temp_alloc(100);
            printf("start1 = %p\n", start1);
            const char *p1 = cook_temp_strdup("Hello, World");
            printf("scope: p1 = %s\n", p1);
            const char *p2 = cook_temp_strndup(p1, 6);
            printf("scope: p2 = %s\n", p2);
            const char *p3 = cook_temp_strsub(p1, 1, 3);
            printf("scope: p3 = %s\n", p3);
            const char *p4 = cook_temp_strfmt("name = %s\nage = %d", "aris", 1);
            printf("scope: p4 = %s\n", p4);
        }
        void *start2 = cook_temp_alloc(200);
        printf("start2 = %p\n", start2);
    }
    void *p1 = cook_temp_alloc(100);
    printf("p1 = %p\n", p1);
    cook_temp_reset();
    void *p2 = cook_temp_alloc(100);
    printf("p2 = %p\n", p2);

    return 0;
}
