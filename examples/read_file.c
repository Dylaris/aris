#define ARIS_IMPLEMENTATION
#include "aris.h"

#include <assert.h>

int main(void)
{
    assert(aris_fload("text") == NULL);

    char *content;

    content = aris_fload(__FILE__);
    printf("size: %zu\ncontent: %s", aris_fsize(content), content);

    aris_funload(content);

    return 0;
}
