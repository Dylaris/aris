#define ARIS_IMPLEMENTATION
#include "aris.h"

#include <assert.h>

int main(void)
{
    assert(aris_file_read("text", ARIS_TEXT_FILE) == NULL);

    char *content;

    content = aris_file_read(__FILE__, ARIS_TEXT_FILE);
    printf("size: %zu\ncontent: %s", aris_file_size(content), content);

    aris_file_free(content);

    return 0;
}
