#ifndef ARIS_H
#define ARIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

/*
 * dynamic array
 */
struct aris_vector_header {
    size_t size;
    size_t capacity;
};

#define aris_vechdr(vec) \
    ((struct aris_vector_header*)((char*)(vec) - sizeof(struct aris_vector_header)))
#define aris_vecsize(vec) ((vec) ? aris_vechdr(vec)->size : 0)
#define aris_veccap(vec)  ((vec) ? aris_vechdr(vec)->capacity : 0)
#define aris_vecpush(vec, item)                                                  \
    do {                                                                         \
        if (aris_vecsize(vec) + 1 > aris_veccap(vec)) {                          \
            size_t newcap, allocsz;                                              \
            struct aris_vector_header *newhdr;                                   \
                                                                                 \
            newcap = aris_veccap(vec) == 0 ? 16 : 2 * aris_veccap(vec);          \
            allocsz = sizeof(struct aris_vector_header) + newcap*sizeof(*(vec)); \
                                                                                 \
            if (vec) {                                                           \
                newhdr = realloc(aris_vechdr(vec), allocsz);                     \
            } else {                                                             \
                newhdr = malloc(allocsz);                                        \
                newhdr->size = 0;                                                \
            }                                                                    \
            newhdr->capacity = newcap;                                           \
                                                                                 \
            (vec) = (void*)((char*)newhdr + sizeof(struct aris_vector_header));  \
        }                                                                        \
                                                                                 \
        (vec)[aris_vechdr(vec)->size++] = (item);                                \
    } while (0)
#define aris_vecpop(vec) ((vec)[--aris_vechdr(vec)->size])
#define aris_vecfree(vec)                \
    do {                                 \
        if (vec) free(aris_vechdr(vec)); \
        (vec) = NULL;                    \
    } while (0)
#define aris_vecreset(vec) ((vec) ? aris_vechdr(vec)->size = 0 : 0)


/*
 * static array
 */
#define aris_arrlen(arr) (sizeof(arr)/sizeof(arr[0]))


/*
 * string
 */
char *aris_strfmt(const char *fmt, ...);
void aris_strrev(char *s);
char *aris_strrev2(const char *s);
bool aris_strpre(const char *s, const char *prefix);
bool aris_strpost(const char *s, const char *postfix);

/*
 * log
 */
#define aris_todo(msg)                                                 \
    do {                                                               \
        fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, msg); \
        abort();                                                       \
    } while(0)
#define aris_unreachable(msg)                                                 \
    do {                                                                      \
        fprintf(stderr, "%s:%d: UNREACHABLE: %s\n", __FILE__, __LINE__, msg); \
        abort();                                                              \
    } while(0)
#define aris_fatal(msg)                                                 \
    do {                                                                \
        fprintf(stderr, "%s:%d: FATAL: %s\n", __FILE__, __LINE__, msg); \
        exit(1);                                                        \
    } while(0)
#define aris_error(msg) \
    fprintf(stderr, "%s:%d: ERROR: %s\n", __FILE__, __LINE__, msg);


/*
 * file
 */
#define aris_fsize(bufp) ((bufp)                                     \
                        ? *(size_t*)((char*)(bufp) - sizeof(size_t)) \
                        : (size_t)0)
#define aris_funload(bufp)                              \
    do {                                                \
        if (!bufp) break;                               \
        char *allocp = (char*)(bufp) - sizeof(size_t);  \
        free(allocp);                                   \
        (bufp) = NULL;                                  \
    } while (0)
char *aris_fload(const char *filename);


/*
 * compiler
 */
#define aris_unused(value) (void)(value)


/*
 * math
 */
#define aris_min(a, b) ((a) < (b) ? (a) : (b))
#define aris_max(a, b) ((a) > (b) ? (a) : (b))
#define aris_swap(a, b) do { (a) ^= (b); (b) ^= (a); (a) ^= (b); } while (0)


/*
 * variable parameter
 */
#define aris_num_vargs(type, ...) (sizeof((type[]){__VA_ARGS__})/sizeof(type))


/*
 * memory layout
 */
#define aris_offset_of(type, member) ((size_t)&(((type*)0)->member))
#define aris_container_of(ptr, type, member) \
    ((type*)((char*)(ptr) - aris_offset_of(type, member)))

#endif /* ARIS_H */

#ifdef ARIS_IMPLEMENTATION

#define ARIS_TMPBUF_SIZE 4096
static char aris__tmpbuf[ARIS_TMPBUF_SIZE];

char *aris_strfmt(const char *fmt, ...)
{
    if (!fmt) return NULL;

    va_list args;
    int len;

    va_start(args, fmt);
    len = vsnprintf(aris__tmpbuf, ARIS_TMPBUF_SIZE, fmt, args);
    va_end(args);

    if (len < 0) return NULL;
    if (len >= ARIS_TMPBUF_SIZE) aris__tmpbuf[ARIS_TMPBUF_SIZE - 1] = '\0';

    return aris__tmpbuf;
}

void aris_strrev(char *s)
{
    if (!s) return;

    for (size_t i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
}

char *aris_strrev2(const char *s)
{
    if (!s) return NULL;

    size_t pos, len;

    len = strlen(s);
    if (len == 0) {
        aris__tmpbuf[0] = '\0';
        return aris__tmpbuf;
    }
    if (len >= ARIS_TMPBUF_SIZE) len = ARIS_TMPBUF_SIZE - 1;

    pos = len;
    aris__tmpbuf[pos--] = '\0';

    for (size_t i = 0; i < len; i++) {
        aris__tmpbuf[pos--] = s[i];
    }

    return aris__tmpbuf;
}

bool aris_strpre(const char *s, const char *prefix)
{
    if (!s || !prefix) return false;

    if (*prefix == '\0') return true;

    while (*prefix) {
        if (*s != *prefix) return false;
        if (*s == '\0') return false;
        s++;
        prefix++;
    }

    return true;
}

bool aris_strpost(const char *s, const char *postfix)
{
    if (!s || !postfix) return false;

    if (*postfix == '\0') return true;

    size_t s_len, postfix_len;

    s_len = strlen(s);
    postfix_len = strlen(postfix);

    if (postfix_len > s_len) return false;

    return memcmp(s + s_len - postfix_len, postfix, postfix_len) == 0;
}

char *aris_fload(const char *filename)
{
    FILE *fp;
    size_t *sizep;
    char *bufp;

    fp = fopen(filename, "r");
    if (!fp) return NULL;

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    sizep = malloc(sizeof(size_t) + size + 1);
    if (!sizep) return NULL;
    *sizep = (size_t)size;

    bufp = (char*)(sizep + 1);
    if (fread(bufp, size, 1, fp) != 1) {
        free(sizep);
        fclose(fp);
        return NULL;
    }
    bufp[size] = '\0';

    fclose(fp);
    return bufp;
}

#endif /* ARIS_IMPLEMENTATION */

#ifdef ARIS_STRIP_PREFIX

#define vechdr       aris_vechdr
#define vecsize      aris_vecsize
#define veccap       aris_veccap
#define vecpush      aris_vecpush
#define vecpop       aris_vecpop
#define vecfree      aris_vecfree
#define vecreset     aris_vecreset

#define arrlen       aris_arrlen

#define strfmt       aris_strfmt
#define strrev       aris_strrev
#define strrev2      aris_strrev2
#define strpre       aris_strpre
#define strpost      aris_strpost

#define todo         aris_todo
#define unreachable  aris_unreachable
#define fatal        aris_fatal
#define error        aris_error

#define unused       aris_unused

#define min          aris_min
#define max          aris_max
#define swap         aris_swap

#define num_vargs    aris_num_vargs

#define offset_of    aris_offset_of
#define container_of aris_container_of

#define fsize        aris_fsize
#define funload      aris_funload
#define fload        aris_fload

#endif /* ARIS_STRIP_PREFIX */
