#define COOK_IMPLEMENTATION
#define COOK_STRIP_PREFIX
#include "cook.h"

int main(void)
{
    assert(file_read("text") == NULL);

    const char *source = file_read(__FILE__);
    printf("size: %zu\ncontent: %s", file_size(source), source);

    file_split(source);
    printf("nlines: %zu\n", file_nlines(source));

    file_foreach(source, line) {
        printf("[%zu] %.*s\n", line->number, (int)line->length, line->start);
        if (line->number >= 10) break;
    }

    file_free(source);

    return 0;
}
