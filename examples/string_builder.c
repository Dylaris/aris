#define COOK_IMPLEMENTATION
#define COOK_STRIP_PREFIX
#include "cook.h"

int main(void)
{
    Cook_String_Builder sb = NULL;
    Cook_String_View sv;

    sb_append(&sb, "Hello, my age is %d", 10);
    printf("%p\n", vec_end(sb));
    sv = sb_view(&sb);
    printf("sv = '"SV_FMT"'\n", SV_ARG(sv));

    sb_append_sv(&sb, sv);
    printf("%p\n", vec_end(sb));
    sv = sb_view(&sb);
    printf("sv = '"SV_FMT"'\n", SV_ARG(sv));

    sb_append_parts(&sb, "nice to see you", 7);
    printf("%p\n", vec_end(sb));
    sv = sb_view(&sb);
    printf("sv = '"SV_FMT"'\n", SV_ARG(sv));

    sb_free(&sb);

    return 0;
}
