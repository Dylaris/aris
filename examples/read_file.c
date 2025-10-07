#define ARIS_IMPLEMENTATION
#define ARIS_STRIP_PREFIX
#include "aris.h"

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
