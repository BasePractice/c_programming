#ifndef C_PROGRAMMING_PRACTICE_PROPERTY_H
#define C_PROGRAMMING_PRACTICE_PROPERTY_H

#include <stdbool.h>

struct Property {
    char *key;
    char *value;
    struct Property *next;
};

typedef bool (*LIST_CALLBACK)(struct Property *element, void *user_data);

typedef bool (*PAYLOAD_CALLBACK)(char *key, char *value, void *user_data);

struct Property *property_new(char *key, char *value);

void property_it(struct Property *root, LIST_CALLBACK callback, void *user_data);

char *property_get(struct Property *root, const char *key);

void property_add(struct Property *list, char *key, char *value);

void property_destroy(struct Property *list, PAYLOAD_CALLBACK callback, void *user_data);

bool payload_nothing_free(char *key, char *value, void *user_data);

#endif //C_PROGRAMMING_PRACTICE_PROPERTY_H
