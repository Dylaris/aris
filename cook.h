/*
cook.h - v0.07 - Dylaris 2025
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
    v0.07 Remote 'mini hash table'
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

#ifndef COOK_VECTOR_INIT_CAPACITY
#define COOK_VECTOR_INIT_CAPACITY 16
#endif

#ifndef COOK_TEMP_BUFFER_CAPACITY
#define COOK_TEMP_BUFFER_CAPACITY (1024*8)
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
/////////////////////// static array
//////////////////////////////////////////////////////

// cook_arr_size - get the vector size from data pointer
// @arr: array
//
// Return: size of array
#define cook_arr_size(arr) (sizeof(arr)/sizeof(arr[0]))

// cook_arr_end - get the end pointer of array
// @arr: array
//
// Return: pointer past the last element
#define cook_arr_end(arr) ((arr)+cook_arr_size(arr))

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
//     int *numbers = NULL;
//     ...
//     cook_arr_foreach(int, numbers, iter) {
//         printf("%d ", *iter);
//     }
// ```
#define cook_arr_foreach(type, arr, iter) for (type *iter = (arr); iter < cook_arr_end(arr); iter++)

//////////////////////////////////////////////////////
/////////////////////// dynamic array
//////////////////////////////////////////////////////

typedef struct cook_vector_header {
    size_t size;
    size_t capacity;
} cook_vechdr_t;

#define COOK_VEC_INITIALIZER NULL

// cook_vec_header - get the vector header from data pointer
// @vec: pointer to vector data
//
// Note: This macro performs pointer arithmetic to get the header structure
//       that precedes the vector data in memory. It's useful for directly
//       manipulating vector metadata.
//
// Example:
// ```
//     int *numbers = NULL; // vector
//     ...
//     cook_vec_header(numbers)->size = 0;
//     cook_vec_header(numbers)->capacity = 0;
// ```
//
// Return: pointer to the cook_vechdr_t structure
#define cook_vec_header(vec) ((vec) ? (cook_vechdr_t*)((char*)(vec)-sizeof(cook_vechdr_t)) : NULL)

// cook_vec_size - get the vector size from data pointer
// @vec: pointer to vector data
//
// Return: size of vector
#define cook_vec_size(vec) ((vec) ? cook_vec_header(vec)->size : 0)

// cook_vec_capacity - get the vector capacity from data pointer
// @vec: pointer to vector data
//
// Return: capacity of vector
#define cook_vec_capacity(vec) ((vec) ? cook_vec_header(vec)->capacity : 0)

// cook_vec_push - push an item to vector
// @vec: pointer to vector data
// @item: element to push
//
// Example:
// ```
//     int *numbers = NULL; // vector
//     cook_vec_push(numbers, 2); // numbers[0] = 2
//     cook_vec_push(numbers, 4); // numbers[1] = 4
// ```
//
// Return: void (updates @vec pointer if reallocation occurs)
#define cook_vec_push(vec, item)                                        \
    do {                                                                \
        if (cook_vec_size(vec) + 1 > cook_vec_capacity(vec)) {          \
            size_t new_capacity =                                       \
                cook_vec_capacity(vec) < COOK_VECTOR_INIT_CAPACITY      \
                ? COOK_VECTOR_INIT_CAPACITY : 2*cook_vec_capacity(vec); \
            (vec) = cook_vec_resize(vec, new_capacity, sizeof(*(vec))); \
            COOK_ASSERT((vec) && "out of memory");                      \
        }                                                               \
        (vec)[cook_vec_header(vec)->size++] = (item);                   \
    } while (0)

// cook_vec_pop - pop an item from vector
// @vec: pointer to vector data
//
// Example:
// ```
//     int *numbers = NULL; // vector
//     cook_vec_push(numbers, 2);     // numbers[0] = 2
//     int n = cook_vec_pop(numbers); // numbers.size = 0, n = 2
// ```
//
// Return: the last element in vector
#define cook_vec_pop(vec) ((vec)[--cook_vec_header(vec)->size])

// cook_vec_empty - check if vector is empty
// @vec: pointer to vector data
//
// Example:
// ```
//     int *numbers = NULL; // vector
//     if (!cook_vec_empty(numbers)) { ... }
// ```
//
// Return: true if vector is empty, false otherwise
#define cook_vec_empty(vec) (cook_vec_size(vec) == 0)

// cook_vec_full - check if vector is full
// @vec: pointer to vector data
//
// Example:
// ```
//     int *numbers = NULL;
//     if (cook_vec_full(numbers)) { ... }
// ```
//
// Return: true if vector is full, false otherwise
#define cook_vec_full(vec) (cook_vec_size(vec) == cook_vec_capacity(vec))

// cook_vec_end - get the end pointer of vector
// @vec: pointer to vector data
//
// Example:
// ```
//     int *numbers = NULL;
//     // iterate through vector
//     for (int *p = numbers; p < cook_vec_end(numbers); p++) {
//         printf("%d ", *p);
//     }
// ```
//
// Return: pointer past the last element
#define cook_vec_end(vec) ((vec) ? (vec)+cook_vec_size(vec) : NULL)

// cook_vec_grow - grow the capacity of vector
// @vec: pointer to vector data
// @cap: additional capacity to grow
//
// Example:
// ```
//     int *numbers = NULL;
//     cook_vec_grow(numbers, 5); // numbers.capacity += 5
// ```
//
// Return: void (updates @vec pointer if reallocation occurs)
#define cook_vec_grow(vec, cap)                                \
    do {                                                       \
        size_t new_cap = cook_vec_capacity(vec) + (cap);       \
        (vec) = cook_vec_resize((vec), new_cap, sizeof(*vec)); \
        COOK_ASSERT((vec) && "out of memory");                 \
    } while (0)

// cook_vec_free - free the memory of vector
// @vec: pointer to vector data
//
// Note: free the memory allocated for vector (including header),
//       and set pointer to NULL
//
// Example:
// ```
//     int *numbers = NULL;
//     // ... use vector ...
//     cook_vec_free(numbers); // numbers is set to NULL
// ```
#define cook_vec_free(vec)                        \
    do {                                          \
        if (vec) COOK_FREE(cook_vec_header(vec)); \
        (vec) = NULL;                             \
    } while (0)

// cook_vec_reset - reset the size of vector to zero
// @vec: pointer to vector data
//
// Note: set the size of vector to zero, but keep the capacity and memory
//       this does not free memory, just reset size
//
// Example:
// ```
//     int *numbers = NULL;
//     cook_vec_push(numbers, 1);
//     cook_vec_push(numbers, 2);
//     cook_vec_reset(numbers); // numbers.size = 0
// ```
#define cook_vec_reset(vec)                      \
    do {                                         \
        if (vec) cook_vec_header(vec)->size = 0; \
    } while (0)

// cook_vec_foreach - iterate through vector
// @type: element type of vector
// @vec: pointer to vector data
// @iter: iterator variable name
//
// Note: a convenience macro to iterate through vector elements
//       the @iter variable will be declared as type* and iterate all elements
//
// Example:
// ```
//     int *numbers = NULL;
//     ...
//     cook_vec_foreach(int, numbers, iter) {
//         printf("%d ", *iter);
//     }
// ```
#define cook_vec_foreach(type, vec, iter) for (type *iter = (vec); iter < cook_vec_end(vec); iter++)

// cook_vec_resize - resize the capacity of vector
// @vec: pointer to vector data
// @new_capacity: new capacity to set
// @item_size: size of each element in bytes
//
// Note: resize the vector to new capacity, if new capacity is less than current size,
//       the vector will be truncated, grow otherwise
//
// Example:
// ```
//     int *numbers = NULL;
//     numbers = cook_vec_resize(numbers, 20, sizeof(int)); // numbers.capacity = 20
// ```
//
// Return: new pointer to vector data, or NULL if allocation failed
COOKDEF void *cook_vec_resize(void *vec, size_t new_capacity, size_t item_size);


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

typedef struct cook_string_view_t {
    const char *data;
    size_t length;
} cook_string_view_t;

#define SV_FMT "%.*s"
#define SV_ARG(sv) (int)(sv).length, (sv).data

// cook_sv_from_cstr - create string view from C string
// @cstr: null-terminated C string
//
// Return: string view representing the C string
COOKDEF cook_string_view_t cook_sv_from_cstr(const char *cstr);

// cook_sv_from_parts - create string view from data pointer and length
// @data: pointer to string data
// @length: length of the string data
//
// Example:
// ```
//     const char *buffer = "hello world";
//     cook_string_view_t sv = cook_sv_from_parts(buffer, 5);
//     // sv.data = "hello", sv.len = 5
// ```
//
// Return: string view representing the specified data range
COOKDEF cook_string_view_t cook_sv_from_parts(const char *data, size_t length);

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
    char *data; // vector of char
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
// @length: length of data in bytes
COOKDEF void cook_sb_append_parts(cook_string_builder_t *sb, const char *data, size_t length);

// cook_sb_reset - reset string builder to empty state
// @sb: string builder pointer
//
// Note: reset string builder's length to zero, but keep allocated buffer
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
// @sb_ptr: pointer to string builder
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
#define cook_sb_append(sb_ptr, fmt, ...)                         \
    do {                                                         \
        size_t checkpoint = cook_temp_save();                    \
        const char *cstr = cook_temp_strfmt(fmt, ##__VA_ARGS__); \
        size_t length = strlen(cstr);                            \
        size_t size = COOK_ALIGN_UP(length, sizeof(void*));      \
        cook_vec_grow((sb_ptr)->data, size);                     \
        memcpy(cook_vec_end((sb_ptr)->data), cstr, length);      \
        cook_vec_header((sb_ptr)->data)->size += length;         \
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

// cook_temp_strndup - duplicate limited length of C string to temporary memory
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

COOKDEF void *cook_vec_resize(void *vec, size_t new_capacity, size_t item_size) {
    size_t alloc_size = sizeof(cook_vechdr_t) + new_capacity*item_size;
    size_t vec_size = cook_vec_size(vec);
    cook_vechdr_t *new_header = COOK_REALLOC(cook_vec_header(vec), alloc_size);
    if (!new_header) return NULL;
    new_header->size = vec_size < new_capacity ? vec_size : new_capacity;
    new_header->capacity = new_capacity;
    return (void*)((char*)new_header + sizeof(cook_vechdr_t));
}

COOKDEF cook_string_view_t cook_sv_from_cstr(const char *cstr) {
    return (cook_string_view_t) {
        .data = cstr,
        .length = cstr ? strlen(cstr) : 0
    };
}

COOKDEF cook_string_view_t cook_sv_from_parts(const char *data, size_t length) {
    return (cook_string_view_t) {
        .data = data,
        .length = length
    };
}

COOKDEF bool cook_sv_equal(cook_string_view_t a, cook_string_view_t b) {
    if (a.length == b.length && memcmp(a.data, b.data, a.length) == 0) {
        return true;
    } else {
        return false;
    }
}

COOKDEF bool cook_sv_starts_with(cook_string_view_t sv, cook_string_view_t prefix) {
    if (prefix.length > sv.length) return false;
    return memcmp(sv.data, prefix.data, prefix.length) == 0;
}

COOKDEF bool cook_sv_ends_with(cook_string_view_t sv, cook_string_view_t postfix) {
    if (postfix.length > sv.length) return false;
    size_t offset = sv.length - postfix.length;
    return memcmp(sv.data + offset, postfix.data, postfix.length) == 0;
}

COOKDEF bool cook_sv_empty(cook_string_view_t sv) {
    return sv.data == NULL || sv.length == 0;
}

COOKDEF cook_string_view_t cook_sv_slice(cook_string_view_t sv, size_t begin, size_t end) {
    cook_string_view_t res = {0};

    if (begin >= end || end - begin > sv.length) return res;
    res.data = sv.data + begin;
    res.length = end - begin;
    return res;
}

COOKDEF cook_string_view_t cook_sv_ltrim(cook_string_view_t sv) {
    cook_string_view_t res = {0};
    for (size_t i = 0; i < sv.length; i++) {
        if (sv.data[i] == ' ' || sv.data[i] == '\t') continue;
        res.data = sv.data + i;
        res.length = sv.length - i;
        break;
    }
    return res;
}

COOKDEF cook_string_view_t cook_sv_rtrim(cook_string_view_t sv) {
    cook_string_view_t res = {0};
    for (size_t i = sv.length; i > 0; i--) {
        if (sv.data[i-1] == ' ' || sv.data[i-1] == '\t') continue;
        res.data = sv.data;
        res.length = i;
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
    for (size_t i = sv.length; i > 0; i--) {
        if (sv.data[i-1] == '\n' || sv.data[i-1] == '\r') continue;
        res.data = sv.data;
        res.length = i;
        break;
    }
    return res;
}

COOKDEF void cook_sb_append_sv(cook_string_builder_t *sb, cook_string_view_t sv) {
    size_t size = COOK_ALIGN_UP(sv.length, sizeof(void*));
    cook_vec_grow(sb->data, size);
    memcpy(cook_vec_end(sb->data), sv.data, sv.length);
    cook_vec_header(sb->data)->size += sv.length;
}

COOKDEF void cook_sb_append_parts(cook_string_builder_t *sb, const char *data, size_t length) {
    size_t size = COOK_ALIGN_UP(length, sizeof(void*));
    cook_vec_grow(sb->data, size);
    memcpy(cook_vec_end(sb->data), data, length);
    cook_vec_header(sb->data)->size += length;
}

COOKDEF void cook_sb_reset(cook_string_builder_t *sb) {
    cook_vec_reset(sb->data);
}

COOKDEF void cook_sb_free(cook_string_builder_t *sb) {
    cook_vec_free(sb->data);
}

COOKDEF cook_string_view_t cook_sb_view(const cook_string_builder_t *sb) {
    return (cook_string_view_t) {
        .data = sb->data,
        .length = cook_vec_size(sb->data)
    };
}

static unsigned char _temp_buffer[COOK_TEMP_BUFFER_CAPACITY] = {0};
static size_t _temp_buffer_used = 0;

COOKDEF void *cook_temp_alloc(size_t size) {
    if (size == 0) return NULL;
    size_t aligned_used = COOK_ALIGN_UP(_temp_buffer_used, sizeof(void*));
    if (aligned_used + size > COOK_TEMP_BUFFER_CAPACITY) return NULL;
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

    size_t sub_length = end - begin;
    char *ptr = cook_temp_alloc(sub_length + 1);
    COOK_ASSERT(ptr != NULL && "out of temporary buffer");

    for (size_t i = 0; i < sub_length ; i++) {
        ptr[i] = cstr[begin + i];
    }
    ptr[sub_length] = '\0';

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
    char *ptr = cook_temp_alloc(sv.length + 1);
    COOK_ASSERT(ptr != NULL && "out of temporary buffer");
    memcpy(ptr, sv.data, sv.length);
    ptr[sv.length] = '\0';
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

#endif // COOK_IMPLEMENTATION

#ifdef COOK_STRIP_PREFIX

#define vec_header   cook_vec_header
#define vec_size     cook_vec_size
#define vec_capacity cook_vec_capacity
#define vec_push     cook_vec_push
#define vec_pop      cook_vec_pop
#define vec_foreach  cook_vec_foreach
#define vec_resize   cook_vec_resize
#define vec_grow     cook_vec_grow
#define vec_empty    cook_vec_empty
#define vec_full     cook_vec_full
#define vec_end      cook_vec_end
#define vec_free     cook_vec_free
#define vec_reset    cook_vec_reset

#define arr_size    cook_arr_size
#define arr_foreach cook_arr_foreach

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
