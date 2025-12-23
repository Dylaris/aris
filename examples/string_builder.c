#define COOK_IMPLEMENTATION
#define COOK_STRIP_PREFIX
#include "cook.h"

int main(void)
{
    cook_string_builder_t sb = {0};
    cook_string_view_t sv;

    sb_append(&sb, "Hello, my age is %d", 10);
    printf("%p\n", vec_end(&sb));
    sv = sb_view(&sb);
    printf("sv = '"SV_FMT"'\n", SV_ARG(sv));

    sv = sv_from_cstr("good study");
    sb_append_sv(&sb, sv);
    printf("%p\n", vec_end(&sb));
    sv = sb_view(&sb);
    printf("sv = '"SV_FMT"'\n", SV_ARG(sv));

    sb_append_parts(&sb, "nice to see you", 7);
    printf("%p\n", vec_end(&sb));
    sv = sb_view(&sb);
    printf("sv = '"SV_FMT"'\n", SV_ARG(sv));

    sb_free(&sb);

    return 0;
}
