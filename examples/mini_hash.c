#define ARIS_IMPLEMENTATION
#define ARIS_STRIP_PREFIX
#include "aris.h"

typedef struct Student {
    uint32_t id;
    const char *name;
} Student;

static Student students[] = {
    {1001, "James Williams"},
    {1002, "Mary Jones"},
    {1003, "Robert Garcia"},
    {1004, "Patricia Rodriguez"},
    {1005, "John Martinez"},
    {1006, "Jennifer Hernandez"},
    {1007, "Michael Lopez"},
    {1008, "Linda Gonzalez"},
    {1009, "William Wilson"},
    {1010, "Elizabeth Anderson"},
    {1011, "David Thomas"},
    {1012, "Barbara Taylor"},
    {1013, "Richard Moore"},
    {1014, "Susan Jackson"},
    {1015, "Joseph Martin"},
    {1016, "Jessica Lee"},
    {1017, "Thomas Perez"},
    {1018, "Sarah Thompson"},
    {1019, "Charles White"},
    {1020, "Karen Harris"}
};

int main(void)
{
    Aris_Mini_Hash name_lookup = {0};
    Aris_Mini_Hash id_lookup = {0};

    // build hash table
    for (size_t i = 0; i < sizeof(students)/sizeof(students[0]); i++) {
        Student *student = &students[i];
        if (!hash_set(&name_lookup, student->id, (uint32_t)i)) return 1;
        if (!hash_set(&id_lookup, fnv_hash(student->name, strlen(student->name)), student->id)) return 1;
    }

    const char *name;
    uint32_t id, index;

    name = "Michael Lopez";
    if (!hash_get(&id_lookup, fnv_hash(name, strlen(name)), &id)) return 1;
    printf("ID of '%s' is: %u\n", name, id);

    name = "David Thomas";
    if (!hash_get(&id_lookup, fnv_hash(name, strlen(name)), &id)) return 1;
    printf("ID of '%s' is: %u\n", name, id);

    id = 1003;
    if (!hash_get(&name_lookup, id, &index)) return 1;
    printf("Name of %u is: %s\n", id, students[index].name);

    id = 1010;
    if (!hash_get(&name_lookup, id, &index)) return 1;
    printf("Name of %u is: %s\n", id, students[index].name);

    hash_free(&name_lookup);
    hash_free(&id_lookup);

    return 0;
}
