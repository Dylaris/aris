#define COOK_IMPLEMENTATION
#define COOK_STRIP_PREFIX
#include "cook.h"

int main(void)
{
    Cook_String_View sv;

    sv = sv_from_cstr(" Hello, World  ");
    printf("sv = '"SV_FMT"'\n", SV_ARG(sv));

    sv = sv_ltrim(sv);
    printf("sv = '"SV_FMT"'\n", SV_ARG(sv));

    sv = sv_rtrim(sv);
    printf("sv = '"SV_FMT"'\n", SV_ARG(sv));

    sv = sv_slice(sv, 1, 3);
    printf("sv = '"SV_FMT"'\n", SV_ARG(sv));

    return 0;
}
