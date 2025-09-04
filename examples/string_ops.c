#define ARIS_IMPLEMENTATION
#include "aris.h"

int main(void)
{
    char s1[] = "Hello, World";
    char *s2 = strdup(aris_strrev2(s1));
    aris_strrev(s1);
    printf("s1: %s\n", s1);
    printf("s2: %s\n", s2);
    printf("%s", aris_strfmt("PREFIX: %s at %s:%d\n", s1, __FILE__, __LINE__));
    free(s2);

    char *s3 = "This is for you";
    if (!aris_strpre(s3, "Th")) aris_fatal("damn it");
    if (!aris_strpost(s3, " you")) aris_fatal("damn it");
    return 0;
}
