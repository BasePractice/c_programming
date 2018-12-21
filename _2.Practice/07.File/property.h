#ifndef C_PROGRAMMING_PRACTICE_PROPERTY_H
#define C_PROGRAMMING_PRACTICE_PROPERTY_H

#include <stdbool.h>

struct Property {
    char *key;
    char *value;
    struct Property *next;
};

typedef bool (*READ_LINE_CALLBACK)(char *line, void *user_data);

void read_lines(char *filename, READ_LINE_CALLBACK callback, void *user_data);
bool parse_line(char *line, void *user_data);
bool property_dynamic_free(char *key, char *value, void *user_data);

typedef bool (*LIST_CALLBACK)(struct Property *element, void *user_data);

typedef bool (*PAYLOAD_CALLBACK)(char *key, char *value, void *user_data);

struct Property *property_new(char *key, char *value);

void property_it(struct Property *root, LIST_CALLBACK callback, void *user_data);

char *property_get(struct Property *root, const char *key);

void property_delete(struct Property *root, const char *key);

void property_update(struct Property *list, char *key, char *value);

void property_destroy(struct Property *list, PAYLOAD_CALLBACK callback, void *user_data);

bool payload_nothing_free(char *key, char *value, void *user_data);

bool property_print(struct Property *element, void *user_data);

#endif //C_PROGRAMMING_PRACTICE_PROPERTY_H
