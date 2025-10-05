#define ARIS_IMPLEMENTATION
#define ARIS_STRIP_PREFIX
#include "aris.h"

int main(void)
{
    assert(file_read("text", ARIS_TEXT_FILE) == NULL);

    const char *source = file_read(__FILE__, ARIS_TEXT_FILE);
    printf("size: %zu\ncontent: %s", file_size(source), source);

    file_split(source);
    printf("nlines: %zu\n", file_nlines(source));

    file_foreach(source, line) {
        printf("[%zu] %.*s\n", line->number, (int)line->size, line->start);
        if (line->number >= 10) break;
    }

    file_free(source);

    return 0;
}
