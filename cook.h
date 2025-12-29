/*
cook.h - v0.08 - Dylaris 2025
===================================================

BRIEF:
  Pieces of C implemented using C99.

NOTICE:
  Not compatible with C++.

USAGE:
  In exactly one source file, define the implementation macro
  before including this header:
  ```
    #define COOK_IMPLEMENTATION
    #include "cook.h"
  ```
  In other files, just include the header without the macro.

HISTORY:
    v0.08 Support 'mini cmd' and 'mini test'
    v0.07 Remove 'mini hash table'
    v0.06 Support 'temp allocator', 'string view', 'string builder'
    v0.05 Remove 'list', 'deque', 'string', 'file'
    v0.04 Support data-structure 'mini hash table'
    v0.03 Support data-structure 'doubly instrusive linked list'
    v0.02 Support data-structure 'deque'

LICENSE:
  See the end of this file for further details.
*/

#ifndef COOK_H
#define COOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#ifndef COOKDEF
#define COOKDEF
#endif

#ifndef COOK_ALLOC
#define COOK_ALLOC(size) malloc(size)
#endif

#ifndef COOK_REALLOC
#define COOK_REALLOC(ptr, size) realloc(ptr, size)
#endif

#ifndef COOK_FREE
#define COOK_FREE(ptr) free(ptr)
#endif

#ifndef COOK_ASSERT
#include <assert.h>
#define COOK_ASSERT(x) assert(x)
#endif

#ifndef COOK_INIT_CAP
#define COOK_INIT_CAP 128
#endif

#ifndef COOK_TEMP_BUFFER_CAP
#define COOK_TEMP_BUFFER_CAP (1024*8)
#endif

//====================================================
//============== MACRO USAGE GUIDELINES ==============
//====================================================
// When using macros , check these rules:
//
// 1. VALIDITY CHECK
//    - Ensure arguments are non-NULL where required
//    - Verify type compatibility
//    - Check preconditions (e.g., initialized state)
//
// 2. SIDE EFFECT SAFETY
//    - Macros may evaluate arguments multiple times
//    - Avoid: function calls (func()), increment/decrement (var++)
//    - Prefer: simple variables, literal values/ When you use the macros, you need to check if it is valid.

//////////////////////////////////////////////////////
/////////////////////// small utils
//////////////////////////////////////////////////////

#define cook_swap(a, b) do { (a) ^= (b); (b) ^= (a); (a) ^= (b); } while (0)

//////////////////////////////////////////////////////
/////////////////////// static array
//////////////////////////////////////////////////////

// cook_arr_len - get the vector size from data pointer
// @arr: array
//
// Return: size of array
#define cook_arr_len(arr) (sizeof(arr)/sizeof(arr[0]))

// cook_arr_end - get the end pointer of array
// @arr: array
//
// Return: pointer past the last element
#define cook_arr_end(arr) ((arr)+cook_arr_len(arr))

// cook_arr_foreach - iterate through array
// @type: element type of array
// @arr: array
// @iter: iterator variable name
//
// Note: a convenience macro to iterate through array elements
//       the @iter variable will be declared as type* and iterate all elements
//
// Example:
// ```
//     cook_arr_foreach(int, numbers, iter) {
//         printf("%d ", *iter);
//     }
// ```
#define cook_arr_foreach(type, arr, iter) for (type *iter = (arr); iter < cook_arr_end(arr); iter++)

// cook_arr_reverse - reverse all elements in array
// @arr: array
#define cook_arr_reverse(arr)                              \
    do {                                                   \
        for (size_t left = 0, right = cook_arr_len(arr)-1; \
                left < right; left++, right--) {           \
            cook_swap((arr)[left], (arr)[right]);          \
        }                                                  \
    } while (0)

//////////////////////////////////////////////////////
/////////////////////// dynamic array
//////////////////////////////////////////////////////

// Before you use the macros about the dynamic array, you
// need to define the 'struct' with the three fields.
//
// Example:
// ```
//     typedef struct array {
//         int *items;
//         size_t len;
//         size_t cap;
//         // other fields
//     } array_t;
// ```

// cook_vec_reverse - reverse all elements in vector
// @vec: pointer to vector
#define cook_vec_reverse(vec)                                   \
    do {                                                        \
        for (size_t left = 0, right = (vec)->len-1;             \
                left < right; left++, right--) {                \
            cook_swap((vec)->items[left], (vec)->items[right]); \
        }                                                       \
    } while (0)

// cook_vec_grow - grow the vector cap
// @vec: pointer to vector
#define cook_vec_grow(vec)                                                      \
    do {                                                                        \
        (vec)->cap = (vec)->cap < COOK_INIT_CAP ? COOK_INIT_CAP : 2*(vec)->cap; \
        (vec)->items = realloc((vec)->items, (vec)->cap*sizeof(*(vec)->items)); \
        COOK_ASSERT((vec)->items && "out of memory");                           \
    } while (0)

// cook_vec_free - free the vector
// @vec: pointer to vector
#define cook_vec_free(vec)                    \
    do {                                      \
        if ((vec)->items) free((vec)->items); \
        (vec)->items = NULL;                  \
        (vec)->len = 0;                       \
        (vec)->cap = 0;                       \
    } while (0)

// cook_vec_push - push an item to vector
// @vec: pointer to vector
// @item: element to push
#define cook_vec_push(vec, item)                             \
    do {                                                     \
        if ((vec)->len + 1 > (vec)->cap) cook_vec_grow(vec); \
        (vec)->items[(vec)->len++] = (item);                 \
    } while (0)

// cook_vec_pop - pop an item from vector
// @vec: pointer to vector
//
// Return: the last element in vector
#define cook_vec_pop(vec) ((vec)[--(vec)->len])

// cook_vec_end - get the end pointer of vector
// @vec: pointer to vector
//
// Example:
// ```
//     // iterate through vector
//     for (int *p = numbers.items; p < cook_vec_end(numbers); p++) {
//         printf("%d ", *p);
//     }
// ```
//
// Return: pointer past the last element
#define cook_vec_end(vec) ((vec)->items + (vec)->len)

// cook_vec_reset - reset the size of vector to zero
// @vec: pointer to vector
//
// Note: set the size of vector to zero, but keep the cap and memory
//       this does not free memory, just reset size
#define cook_vec_reset(vec) do { (vec)->len = 0; } while (0)

// cook_vec_foreach - iterate through vector
// @type: element type of vector
// @vec: pointer to vector
// @iter: iterator variable name
//
// Note: a convenience macro to iterate through vector elements
//       the @iter variable will be declared as type* and iterate all elements
//
// Example:
// ```
//     cook_vec_foreach(int, numbers, iter) {
//         printf("%d ", *iter);
//     }
// ```
#define cook_vec_foreach(type, vec, iter) for (type *iter = (vec)->items; iter < cook_vec_end(vec); iter++)


//////////////////////////////////////////////////////
/////////////////////// memory layout
//////////////////////////////////////////////////////

// COOK_OFFSET_OF - get the offset of a member in a structure
// @type: structure type
// @member: member name in the structure
//
// Example:
// ```
//     struct person {
//         int id;
//         char name[20];
//         int age;
//     };
//
//     size_t offset = COOK_OFFSET_OF(struct person, age);
// ```
//
// Return: offset in bytes of the member within the structure
#define COOK_OFFSET_OF(type, member) ((size_t)&(((type*)0)->member))

// COOK_CONTAINER_OF - get the container structure from member pointer
// @ptr: pointer to member
// @type: container structure type
// @member: member name in the structure
//
// Example:
// ```
//     struct person {
//         int id;
//         char name[20];
//         int age;
//     };
//
//     struct person john = {1, "John", 30};
//     int *age_ptr = &john.age;
//
//     struct person *person_ptr = COOK_CONTAINER_OF(age_ptr, struct person, age);
//     // person_ptr now points to john, so we can access john.id or john.name
// ```
//
// Return: pointer to the container structure
#define COOK_CONTAINER_OF(ptr, type, member) \
    ((type*)((char*)(ptr)-COOK_OFFSET_OF(type, member)))

// COOK_ALIGN_UP - align value up to the nearest multiple
// @n: value to align
// @k: alignment boundary (must be power of two)
//
// Return: @n aligned up to multiple of @k
#define COOK_ALIGN_UP(n, k) (((n)+(k)-1)&~((k)-1))

// COOK_ALIGN_DOWN - align value down to the nearest multiple
// @n: value to align
// @k: alignment boundary (must be power of two)
//
// Return: @n aligned down to multiple of @k
#define COOK_ALIGN_DOWN(n, k) ((n)&~((k)-1))


//////////////////////////////////////////////////////
/////////////////////// string view
//////////////////////////////////////////////////////

typedef struct cook_string_view {
    const char *data;
    size_t len;
} cook_string_view_t;

#define SV_FMT "%.*s"
#define SV_ARG(sv) (int)(sv).len, (sv).data

// cook_sv_from_cstr - create string view from C string
// @cstr: null-terminated C string
//
// Return: string view representing the C string
COOKDEF cook_string_view_t cook_sv_from_cstr(const char *cstr);

// cook_sv_from_parts - create string view from data pointer and len
// @data: pointer to string data
// @len: len of the string data
//
// Example:
// ```
//     const char *buffer = "hello world";
//     cook_string_view_t sv = cook_sv_from_parts(buffer, 5);
//     // sv.data = "hello", sv.len = 5
// ```
//
// Return: string view representing the specified data range
COOKDEF cook_string_view_t cook_sv_from_parts(const char *data, size_t len);

// cook_sv_equal - check if two string views are equal
// @a: first string view
// @b: second string view
//
// Return: true if string views are equal, false otherwise
COOKDEF bool cook_sv_equal(cook_string_view_t a, cook_string_view_t b);

// cook_sv_starts_with - check if string view starts with prefix
// @sv: string view to check
// @prefix: prefix to look for
//
// Return: true if @sv starts with @prefix, false otherwise
COOKDEF bool cook_sv_starts_with(cook_string_view_t sv, cook_string_view_t prefix);

// cook_sv_ends_with - check if string view ends with postfix
// @sv: string view to check
// @postfix: postfix to look for
//
// Return: true if @sv ends with @postfix, false otherwise
COOKDEF bool cook_sv_ends_with(cook_string_view_t sv, cook_string_view_t postfix);

// cook_sv_empty - check if string view is empty
// @sv: string view to check
//
// Return: true if string view is empty (len == 0), false otherwise
COOKDEF bool cook_sv_empty(cook_string_view_t sv);

// cook_sv_slice - extract substring from string view
// @sv: source string view
// @begin: starting index (inclusive)
// @end: ending index (exclusive)
//
// Note: extract a substring from @sv[begin] to @sv[end-1],
//       if indices are out of bounds, returns empty string view
//
// Return: substring view of the specified range
COOKDEF cook_string_view_t cook_sv_slice(cook_string_view_t sv, size_t begin, size_t end);

// cook_sv_trim - trim whitespace(space/tab) from both ends of string view
// @sv: string view to trim
//
// Return: trimmed string view
COOKDEF cook_string_view_t cook_sv_trim(cook_string_view_t sv);

// cook_sv_ltrim - trim whitespace(space/tab) from left end of string view
// @sv: string view to trim
//
// Return: left-trimmed string view
COOKDEF cook_string_view_t cook_sv_ltrim(cook_string_view_t sv);

// cook_sv_rtrim - trim whitespace(space/tab) from right end of string view
// @sv: string view to trim
//
// Return: right-trimmed string view
COOKDEF cook_string_view_t cook_sv_rtrim(cook_string_view_t sv);

// cook_sv_chomp - remove trailing newline character(s) from string view
// @sv: string view to process
//
// Note: remove trailing newline sequences (\n, \r, \r\n) from the end of string view,
//
// Example:
// ```
//     // Input from fgets() includes newline
//     char buffer[256];
//     fgets(buffer, sizeof(buffer), stdin);
//     cook_string_view_t sv = cook_sv_from_cstr(buffer);
//     sv = cook_sv_chomp(sv);  // Remove the trailing newline
// ```
//
// Return: string view with trailing newline(s) removed
COOKDEF cook_string_view_t cook_sv_chomp(cook_string_view_t sv);


//////////////////////////////////////////////////////
/////////////////////// string builder
//////////////////////////////////////////////////////

// Note: Do not append data that references the builder's own buffer.
//       The buffer may be reallocated during append operations, invalidating
//       any pointers to it.
//
// Example of unsafe usage (DO NOT DO THIS):
// ```
//     cook_string_view_t sv = cook_sb_view(&sb);
//     cook_sb_append_sv(&sb, sv);  // x sv.data may becomes invalid if reallocation occurs!
// ```

typedef struct cook_string_builder {
    char *items;
    size_t len;
    size_t cap;
} cook_string_builder_t;

// cook_sb_append_sv - append string view to string builder
// @sb: string builder pointer
// @sv: string view to append
//
// Note: append the contents of string view to the string builder,
//       the string builder's internal buffer will grow automatically if needed
COOKDEF void cook_sb_append_sv(cook_string_builder_t *sb, cook_string_view_t sv);

// cook_sb_append_parts - append raw data to string builder
// @sb: string builder pointer
// @data: pointer to data to append
// @len: len of data in bytes
COOKDEF void cook_sb_append_parts(cook_string_builder_t *sb, const void *data, size_t len);

// cook_sb_reset - reset string builder to empty state
// @sb: string builder pointer
//
// Note: reset string builder's len to zero, but keep allocated buffer
//       for reuse, memory is not freed
COOKDEF void cook_sb_reset(cook_string_builder_t *sb);

// cook_sb_free - free string builder memory
// @sb: string builder pointer
COOKDEF void cook_sb_free(cook_string_builder_t *sb);

// cook_sb_view - get string view of builder's contents
// @sb: string builder pointer
//
// Return: string view of builder's current contents
COOKDEF cook_string_view_t cook_sb_view(const cook_string_builder_t *sb);

// cook_sb_append - append formatted string to string builder
// @sb: pointer to string builder
// @fmt: format string
// @...: arguments for formatting
//
// Note: append formatted string to string builder using temporary memory,
//       the macro handles memory allocation and copying automatically
//
// Example:
// ```
//     cook_string_builder_t sb = {0};
//     cook_sb_append(&sb, "value: %d", 42);     // sb contains "value: 42"
//     cook_sb_append(&sb, ", name: %s", "test"); // sb contains "value: 42, name: test"
// ```
#define cook_sb_append(sb, fmt, ...)                             \
    do {                                                         \
        size_t checkpoint = cook_temp_save();                    \
        const char *cstr = cook_temp_strfmt(fmt, ##__VA_ARGS__); \
        size_t len = strlen(cstr);                               \
        cook_sb_append_parts(sb, (const void *) cstr, len);      \
        cook_temp_rewind(checkpoint);                            \
    } while (0)


//////////////////////////////////////////////////////
/////////////////////// input/output
//////////////////////////////////////////////////////

typedef struct cook_reader cook_reader_t;
struct cook_reader {
    void *ctx;
    long (*read)(cook_reader_t *r, void *buf, size_t size);
    void (*close)(cook_reader_t *r);
    const char *(*error)(cook_reader_t *r);
};

typedef struct cook_writer cook_writer_t;
struct cook_writer {
    void *ctx;
    long (*write)(cook_writer_t *w, const void *data, size_t size);
    int (*flush)(cook_writer_t *w);
    void (*close)(cook_writer_t *w);
    const char *(*error)(cook_writer_t *w);
};


//////////////////////////////////////////////////////
/////////////////////// mini cmd
//////////////////////////////////////////////////////

typedef cook_string_builder_t cook_cmd_t;

// cook_cmd_append - append a formated string to cmd
// @cmd: pointer to cmd
// @fmt: string format
// @...: variable parameters
#define cook_cmd_append(cmd, fmt, ...)           \
    do {                                         \
        cook_sb_append(cmd, " ");                \
        cook_sb_append(cmd, fmt, ##__VA_ARGS__); \
    } while (0)

// cook_cmd_append_many - append many string to cmd
// @cmd: pointer to cmd
// @...: variable parameters
#define cook_cmd_append_many(cmd, ...)                     \
    do {                                                   \
        const char *parts[] = {__VA_ARGS__};               \
        for (size_t i = 0; i < cook_arr_len(parts); i++) { \
            cook_sb_append(cmd, " ");                      \
            cook_sb_append(cmd, parts[i]);                 \
        }                                                  \
    } while (0)

// cook_cmd_free - free all memory cmd takes
// @cmd: pointer to cmd
COOKDEF void cook_cmd_free(cook_cmd_t *cmd);

// cook_cmd_reset - reset cmd, keep the memory
// @cmd: pointer to cmd
COOKDEF void cook_cmd_reset(cook_cmd_t *cmd);

// cook_cmd_print - print cmd with prefix
// @cmd: pointer to cmd
// @prefix: prefix string
COOKDEF void cook_cmd_print(cook_cmd_t *cmd, const char *prefix);

// cook_cmd_run - run cmd using 'system()'
// @cmd: pointer to cmd
COOKDEF bool cook_cmd_run(cook_cmd_t *cmd);


//////////////////////////////////////////////////////
/////////////////////// mini test
//////////////////////////////////////////////////////

typedef struct {
    const char *expr;
    const char *info;
    const char *file;
    size_t line;
    size_t id;
    bool ok;
} cook_mucase_t;

typedef struct {
    const char *name;
    cook_mucase_t *items;
    size_t len;
    size_t cap;
    size_t passed;
    size_t failed;
} cook_musuite_t;

#define COOK_MUTEST(expr_, info_)               \
    do {                                        \
        cook_mucase_t test_case = {             \
            .expr = # expr_,                    \
            .info = info_,                      \
            .file = __FILE__,                   \
            .line = __LINE__,                   \
            .id = _test_suite.len + 1,          \
            .ok = (expr_)                       \
        };                                      \
        cook_vec_push(&_test_suite, test_case); \
        if (test_case.ok) _test_suite.passed++; \
        else _test_suite.failed++;              \
    } while (0)

// cook_mutest_detail - print details (all test cases)
COOKDEF void cook_mutest_detail(void);

// cook_mutest_summary - print summary (only summary)
COOKDEF void cook_mutest_summary(void);

// helper function for testing
COOKDEF bool cook_expect_mem_eq(size_t n, void *p1, void *p2);
COOKDEF bool cook_expect_str_eq(const char *s1, const char *s2);


//////////////////////////////////////////////////////
/////////////////////// temporary allocator
/////////////////////// (steal from https://github.com/tsoding/nob.h.git)
//////////////////////////////////////////////////////

// cook_temp_alloc - allocate temporary memory
// @size: size in bytes to allocate
//
// Return: pointer to allocated memory, or NULL if allocation failed
COOKDEF void *cook_temp_alloc(size_t size);

// cook_temp_strdup - duplicate C string to temporary memory
// @cstr: null-terminated C string to duplicate
//
// Return: pointer to duplicated string in temporary memory
COOKDEF const char *cook_temp_strdup(const char *cstr);

// cook_temp_strndup - duplicate limited len of C string to temporary memory
// @cstr: null-terminated C string to duplicate
// @n: maximum number of characters to copy
//
// Return: pointer to duplicated string in temporary memory
COOKDEF const char *cook_temp_strndup(const char *cstr, size_t n);

// cook_temp_strsub - extract substring to temporary memory
// @cstr: null-terminated C string
// @begin: starting index (inclusive)
// @end: ending index (exclusive)
//
// Note: extract substring from @cstr[begin] to @cstr[end-1] to temporary memory,
//
// Return: pointer to substring in temporary memory
COOKDEF const char *cook_temp_strsub(const char *cstr, size_t begin, size_t end);

// cook_temp_strfmt - format string to temporary memory
// @fmt: format string
// @...: arguments for format
//
// Note: format string using printf-style formatting to temporary memory,
//
// Example:
// ```
//     const char *msg = cook_temp_strfmt("value: %d, name: %s", 42, "test");
//     // msg is "value: 42, name: test"
// ```
//
// Return: pointer to formatted string in temporary memory
COOKDEF const char *cook_temp_strfmt(const char *fmt, ...);

// cook_temp_sv2cstr - convert string view to C string in temporary memory
// @sv: string view to convert
//
// Note: convert string view to null-terminated C string in temporary memory,
//
// Example:
// ```
//     cook_string_view_t sv = {.data = "hello", .len = 5};
//     const char *cstr = cook_temp_sv2cstr(sv);
//     // cstr is "hello" (null-terminated)
// ```
//
// Return: pointer to C string in temporary memory
COOKDEF const char *cook_temp_sv2cstr(cook_string_view_t sv);

// cook_temp_save - save current temporary memory state
//
// Note: save current position in temporary memory arena, which can be used
//       later with cook_temp_rewind() to free memory allocated after this point
//
// Example:
// ```
//     size_t checkpoint = cook_temp_save();
//     const char *str1 = cook_temp_strdup("first");
//     const char *str2 = cook_temp_strdup("second");
//
//     cook_temp_rewind(checkpoint); // str1 and str2 are freed
// ```
//
// Return: checkpoint ID that can be used with cook_temp_rewind()
COOKDEF size_t cook_temp_save(void);

// cook_temp_rewind - rewind temporary memory to checkpoint
// @checkpoint: checkpoint returned by cook_temp_save()
//
// Note: rewind temporary memory arena to the state saved at @checkpoint,
//       all memory allocated after that checkpoint will be freed
//
// Example:
// ```
//     size_t checkpoint = cook_temp_save();
//     cook_temp_strdup("temporary");
//     cook_temp_rewind(checkpoint); // memory is freed
// ```
COOKDEF void cook_temp_rewind(size_t checkpoint);

// cook_temp_reset - reset all temporary memory
//
// Note: free all memory allocated from temporary memory arena,
//       reset the arena to initial state
//
// Example:
// ```
//     cook_temp_strdup("string1");
//     cook_temp_strdup("string2");
//     cook_temp_reset(); // all temporary memory is freed
// ```
COOKDEF void cook_temp_reset(void);

// cook_temp_path_join - join two paths using temporary memory
// @path1: first path component
// @path2: second path component
//
// Example:
// ```
//     const char *path = cook_temp_path_join("/home/user", "docs/file.txt");
//     // path = "/home/user/docs/file.txt" (in temporary memory)
// ```
//
// Return: joined path in temporary memory, or NULL on error
COOKDEF const char *cook_temp_path_join(const char *path1, const char *path2);

// cook_temp_path_dirname - get directory name using temporary memory
// @path: file path
//
// Example:
// ```
//     const char *dir = cook_temp_path_dirname("/home/user/file.txt");
//     // dir = "/home/user" (in temporary memory)
// ```
//
// Return: directory name in temporary memory, or "." if no directory
COOKDEF const char *cook_temp_path_dirname(const char *path);

// cook_temp_path_basename - get base filename using temporary memory
// @path: file path
//
// Example:
// ```
//     const char *file = cook_temp_path_basename("/home/user/file.txt");
//     // file = "file.txt" (in temporary memory)
// ```
//
// Return: base filename in temporary memory, or empty string if no filename
COOKDEF const char *cook_temp_path_basename(const char *path);

#endif // COOK_H

#ifdef COOK_IMPLEMENTATION

COOKDEF cook_string_view_t cook_sv_from_cstr(const char *cstr) {
    return (cook_string_view_t) {
        .data = cstr,
        .len = cstr ? strlen(cstr) : 0
    };
}

COOKDEF cook_string_view_t cook_sv_from_parts(const char *data, size_t len) {
    return (cook_string_view_t) {
        .data = data,
        .len = len
    };
}

COOKDEF bool cook_sv_equal(cook_string_view_t a, cook_string_view_t b) {
    if (a.len == b.len && memcmp(a.data, b.data, a.len) == 0) {
        return true;
    } else {
        return false;
    }
}

COOKDEF bool cook_sv_starts_with(cook_string_view_t sv, cook_string_view_t prefix) {
    if (prefix.len > sv.len) return false;
    return memcmp(sv.data, prefix.data, prefix.len) == 0;
}

COOKDEF bool cook_sv_ends_with(cook_string_view_t sv, cook_string_view_t postfix) {
    if (postfix.len > sv.len) return false;
    size_t offset = sv.len - postfix.len;
    return memcmp(sv.data + offset, postfix.data, postfix.len) == 0;
}

COOKDEF bool cook_sv_empty(cook_string_view_t sv) {
    return sv.data == NULL || sv.len == 0;
}

COOKDEF cook_string_view_t cook_sv_slice(cook_string_view_t sv, size_t begin, size_t end) {
    cook_string_view_t res = {0};

    if (begin >= end || end - begin > sv.len) return res;
    res.data = sv.data + begin;
    res.len = end - begin;
    return res;
}

COOKDEF cook_string_view_t cook_sv_ltrim(cook_string_view_t sv) {
    cook_string_view_t res = {0};
    for (size_t i = 0; i < sv.len; i++) {
        if (sv.data[i] == ' ' || sv.data[i] == '\t') continue;
        res.data = sv.data + i;
        res.len = sv.len - i;
        break;
    }
    return res;
}

COOKDEF cook_string_view_t cook_sv_rtrim(cook_string_view_t sv) {
    cook_string_view_t res = {0};
    for (size_t i = sv.len; i > 0; i--) {
        if (sv.data[i-1] == ' ' || sv.data[i-1] == '\t') continue;
        res.data = sv.data;
        res.len = i;
        break;
    }
    return res;
}

COOKDEF cook_string_view_t cook_sv_trim(cook_string_view_t sv) {
    cook_string_view_t res;
    res = cook_sv_ltrim(sv);
    res = cook_sv_rtrim(res);
    return res;
}

COOKDEF cook_string_view_t cook_sv_chomp(cook_string_view_t sv) {
    cook_string_view_t res = {0};
    for (size_t i = sv.len; i > 0; i--) {
        if (sv.data[i-1] == '\n' || sv.data[i-1] == '\r') continue;
        res.data = sv.data;
        res.len = i;
        break;
    }
    return res;
}

COOKDEF void cook_sb_append_sv(cook_string_builder_t *sb, cook_string_view_t sv) {
    cook_sb_append_parts(sb, sv.data, sv.len);
}

COOKDEF void cook_sb_append_parts(cook_string_builder_t *sb, const void *data, size_t len) {
    if (sb->len + 1 > sb->cap) cook_vec_grow(sb);
    memcpy(cook_vec_end(sb), data, len);
    sb->len += len;
}

COOKDEF void cook_sb_reset(cook_string_builder_t *sb) {
    cook_vec_reset(sb);
}

COOKDEF void cook_sb_free(cook_string_builder_t *sb) {
    cook_vec_free(sb);
}

COOKDEF cook_string_view_t cook_sb_view(const cook_string_builder_t *sb) {
    return (cook_string_view_t) {
        .data = sb->items,
        .len = sb->len
    };
}

static unsigned char _temp_buffer[COOK_TEMP_BUFFER_CAP] = {0};
static size_t _temp_buffer_used = 0;

COOKDEF void *cook_temp_alloc(size_t size) {
    if (size == 0) return NULL;
    size_t aligned_used = COOK_ALIGN_UP(_temp_buffer_used, sizeof(void*));
    if (aligned_used + size > COOK_TEMP_BUFFER_CAP) return NULL;
    void *ptr = _temp_buffer + aligned_used;
    _temp_buffer_used = aligned_used + size;
    return ptr;
}

COOKDEF size_t cook_temp_save(void) {
    return _temp_buffer_used;
}

COOKDEF void cook_temp_rewind(size_t checkpoint) {
    _temp_buffer_used = checkpoint;
}

COOKDEF void cook_temp_reset(void) {
    _temp_buffer_used = 0;
}

COOKDEF const char *cook_temp_strdup(const char *cstr) {
    return cook_temp_strsub(cstr, 0, strlen(cstr) + 1);
}

COOKDEF const char *cook_temp_strndup(const char *cstr, size_t n) {
    return cook_temp_strsub(cstr, 0, n);
}

COOKDEF const char *cook_temp_strsub(const char *cstr, size_t begin, size_t end) {
    if (!cstr || begin >= end) return NULL;

    size_t sub_len = end - begin;
    char *ptr = cook_temp_alloc(sub_len + 1);
    COOK_ASSERT(ptr != NULL && "out of temporary buffer");

    for (size_t i = 0; i < sub_len ; i++) {
        ptr[i] = cstr[begin + i];
    }
    ptr[sub_len] = '\0';

    return ptr;
}

COOKDEF const char *cook_temp_strfmt(const char *fmt, ...) {
    if (!fmt) return NULL;

    va_list args;

    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    if (len < 0) return NULL;

    void *ptr = cook_temp_alloc((size_t)len + 1);
    COOK_ASSERT(ptr != NULL && "out of temporary buffer");

    va_start(args, fmt);
    vsnprintf(ptr, (size_t)len + 1, fmt, args);
    va_end(args);

    return ptr;
}

COOKDEF const char *cook_temp_sv_to_cstr(cook_string_view_t sv) {
    char *ptr = cook_temp_alloc(sv.len + 1);
    COOK_ASSERT(ptr != NULL && "out of temporary buffer");
    memcpy(ptr, sv.data, sv.len);
    ptr[sv.len] = '\0';
    return ptr;
}

COOKDEF const char *cook_temp_path_join(const char *path1, const char *path2) {
    if (!path1 || !*path1) return cook_temp_strdup(path2 ? path2 : "");
    if (!path2 || !*path2) return cook_temp_strdup(path1);

    size_t len1 = strlen(path1);
    bool has_sep1 = len1 > 0 && (path1[len1 - 1] == '/' || path1[len1 - 1] == '\\');
    bool has_sep2 = path2[0] == '/' || path2[0] == '\\';

    size_t total_len = len1 + strlen(path2) + 2;
    char *buffer = cook_temp_alloc(total_len);
    if (!buffer) return NULL;

    strcpy(buffer, path1);

    if (!has_sep1 && !has_sep2) {
        // need separator
#ifdef _WIN32
        strcat(buffer, "\\");
#else
        strcat(buffer, "/");
#endif
        strcat(buffer, path2);
    } else if (has_sep1 && has_sep2) {
        // skip leading separator in path2
        strcat(buffer, path2 + 1);
    } else {
        // directly concatenate
        strcat(buffer, path2);
    }

    return buffer;
}

COOKDEF const char *cook_temp_path_dirname(const char *path) {
    if (!path || !*path) return "./";

    // find last separator
    const char *last_sep = NULL;
    const char *p = path;
    while (*p) {
        if (*p == '/' || *p == '\\') last_sep = p;
        p++;
    }

    // no separator found
    if (!last_sep) return "./";

    // copy directory part
    size_t dir_len = last_sep - path + 1;
    char *result = cook_temp_alloc(dir_len + 1);
    if (!result) return NULL;

    memcpy(result, path, dir_len);
    result[dir_len] = '\0';
    return result;
}

COOKDEF const char *cook_temp_path_basename(const char *path) {
    if (!path || !*path) return "";

    // find last separator
    const char *last_sep = NULL;
    const char *p = path;
    while (*p) {
        if (*p == '/' || *p == '\\') last_sep = p;
        p++;
    }

    // no separator, whole path is basename
    if (!last_sep) return cook_temp_strdup(path);

    // return part after last separator
    return cook_temp_strdup(last_sep + 1);
}

COOKDEF void cook_cmd_free(cook_cmd_t *cmd) {
    cook_sb_free(cmd);
}

COOKDEF void cook_cmd_reset(cook_cmd_t *cmd) {
    cook_sb_reset(cmd);
}

COOKDEF void cook_cmd_print(cook_cmd_t *cmd, const char *prefix) {
    cook_string_view_t sv = cook_sb_view(cmd);
    printf("%s"SV_FMT"\n", prefix, SV_ARG(sv));
    fflush(stdout);
}

COOKDEF bool cook_cmd_run(cook_cmd_t *cmd) {
    if (cmd->cap == cmd->len) {
        cook_sb_append(cmd, "");
    } else {
        cmd->items[cmd->len] = '\0';
    }
    cook_cmd_print(cmd, "[INFO]");
    int status = system(cmd->items);
    if (status != 0) cook_cmd_print(cmd, "[ERROR]");
    return status == 0;
}

static cook_musuite_t _test_suite = {
    .name = "default",
    .items = NULL,
    .len = 0,
    .cap = 0
};

// cook__mutest_print_case - print one test case
// @test_case: pointer to tset case
static void cook__mutest_print_case(cook_mucase_t *test_case) {
    printf("[%02ld] [%s] [%s:%ld] >>> %s <<< --- %s\n",
           test_case->id, test_case->ok ? "pass" : "fail",
           test_case->file, test_case->line, test_case->expr, test_case->info);
}

COOKDEF void cook_mutest_detail(void) {
    for (size_t i = 0; i < _test_suite.len; i++) {
        cook__mutest_print_case(&_test_suite.items[i]);
    }
}

COOKDEF void cook_mutest_summary(void) {
    printf("-------------------------------------------------\n");
    printf("Summary:\n");
    printf("    passed: %ld\n", _test_suite.passed);
    printf("    failed: %ld\n", _test_suite.failed);
    printf("    total:  %ld\n", _test_suite.len);
    printf("-------------------------------------------------\n");
    cook_vec_free(&_test_suite);
}

COOKDEF bool cook_expect_mem_eq(size_t n, void *p1, void *p2) {
    return memcmp(p1, p2, n) == 0;
}

COOKDEF bool cook_expect_str_eq(const char *s1, const char *s2) {
    if (!s1 && !s2) return true;  // s1 = NULL, s2 = NULL
    if (!s1 || !s2) return false; // s1 = NULL, s2 != NULL
    return strcmp(s1, s2) == 0;
}

#endif // COOK_IMPLEMENTATION

#ifdef COOK_STRIP_PREFIX

typedef cook_string_view_t string_view_t;
typedef cook_string_builder_t string_builder_t;
typedef cook_cmd_t cmd_t;
typedef cook_mucase_t mucase_t;
typedef cook_musuite_t musuite_t;

#define vec_push     cook_vec_push
#define vec_pop      cook_vec_pop
#define vec_foreach  cook_vec_foreach
#define vec_grow     cook_vec_grow
#define vec_end      cook_vec_end
#define vec_free     cook_vec_free
#define vec_reset    cook_vec_reset
#define vec_reverse  cook_vec_reverse

#define arr_len     cook_arr_len
#define arr_foreach cook_arr_foreach
#define arr_reverse cook_arr_reverse

#define ALIGN_UP     COOK_ALIGN_UP
#define ALIGN_DOWN   COOK_ALIGN_DOWN
#define OFFSET_OF    COOK_OFFSET_OF
#define CONTAINER_OF COOK_CONTAINER_OF

#define temp_alloc         cook_temp_alloc
#define temp_strdup        cook_temp_strdup
#define temp_strndup       cook_temp_strndup
#define temp_strsub        cook_temp_strsub
#define temp_strfmt        cook_temp_strfmt
#define temp_save          cook_temp_save
#define temp_rewind        cook_temp_rewind
#define temp_reset         cook_temp_reset
#define temp_path_join     cook_temp_path_join
#define temp_path_dirname  cook_temp_path_dirname
#define temp_path_basename cook_temp_path_basename

#define sv_from_cstr   cook_sv_from_cstr
#define sv_from_parts  cook_sv_from_parts
#define sv_equal       cook_sv_equal
#define sv_compare     cook_sv_compare
#define sv_starts_with cook_sv_starts_with
#define sv_ends_with   cook_sv_ends_with
#define sv_empty       cook_sv_empty
#define sv_slice       cook_sv_slice
#define sv_trim        cook_sv_trim
#define sv_ltrim       cook_sv_ltrim
#define sv_rtrim       cook_sv_rtrim
#define sv_chomp       cook_sv_chomp

#define sb_append_sv    cook_sb_append_sv
#define sb_append_parts cook_sb_append_parts
#define sb_reset        cook_sb_reset
#define sb_free         cook_sb_free
#define sb_view         cook_sb_view
#define sb_append       cook_sb_append

#define cmd_append      cook_cmd_append
#define cmd_append_many cook_cmd_append_many
#define cmd_reset       cook_cmd_reset
#define cmd_free        cook_cmd_free
#define cmd_run         cook_cmd_run
#define cmd_print       cook_cmd_print

#define MUTEST COOK_MUTEST
#define mutest_detail  cook_mutest_detail
#define mutest_summary cook_mutest_summary
#define expect_mem_eq  cook_expect_mem_eq
#define expect_str_eq  cook_expect_str_eq

#endif // COOK_STRIP_PREFIX

/*
------------------------------------------------------------------------------
This software is available under MIT license.
------------------------------------------------------------------------------
Copyright (c) 2025 Dylaris
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, COOKING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
