#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "property.h"


void property_it(struct Property *root, LIST_CALLBACK callback, void *user_data) {
    struct Property *it = root;

    while (it != 0) {
        bool ret = (*callback)(it, user_data);
        if (ret == false)
            return;
        it = it->next;
    }
}

struct PropertyFind {
    const char *key;
    struct Property *element;
};

static bool property_find(struct Property *element, void *user_data) {
    struct PropertyFind *find = user_data;
    if (strcmp(element->key, find->key) == 0) {
        find->element = element;
        return false;
    }
    return true;
}

char *property_get(struct Property *root, const char *key) {
    struct PropertyFind find = {key, 0};
    property_it(root, property_find, &find);
    return find.element != 0 ? find.element->value : 0;
}

bool property_print(struct Property *element, void *user_data) {
    if (strcmp(element->key, "root") == 0)
        return true;
    fprintf(stdout, "%s = %s\n", element->key, element->value);
    return true;
}

void property_delete(struct Property *root, const char *key) {
    /*TODO: Реализовать самостоятельно */
}

void property_update(struct Property *list, char *key, char *value) {
    struct Property *it = list;
    struct PropertyFind find = {key, 0};
    property_it(list, property_find, &find);
    if (find.element != 0) {
        free(find.element->value);
        free(key);
        find.element->value = value;
    } else {
        while (it != 0 && it->next != 0) {
            it = it->next;
        }
        if (it != 0) {
            it->next = property_new(key, value);
        }
    }
}

struct Property *property_new(char *key, char *value) {
    struct Property *list = calloc(sizeof(struct Property), 1);
    list->key = key;
    list->value = value;
    return list;
}

void property_destroy(struct Property *list, PAYLOAD_CALLBACK callback, void *user_data) {
    struct Property *it = list;

    while (it != 0) {
        struct Property *next = it->next;
        (*callback)(it->key, it->value, user_data);
        free(it);
        it = next;
    }
}

bool payload_nothing_free(char *key, char *value, void *user_data) {
    return true;
}

#define LINE_READ_BUFFER_SIZE 10

void read_lines(char *filename, READ_LINE_CALLBACK callback, void *user_data) {
    FILE *fd = fopen(filename, "r");
    if (fd != 0) {
        bool next = true;
        int ch;
        char *line;
        size_t line_allocated_size = 0;
        size_t line_it = 0;

        line_allocated_size = LINE_READ_BUFFER_SIZE;
        line = calloc(LINE_READ_BUFFER_SIZE, 1);

        while (next && (ch = fgetc(fd)) != EOF) {
            if (line_it > line_allocated_size) {
                line_allocated_size += LINE_READ_BUFFER_SIZE;
                line = realloc(line, line_allocated_size);
            }
            if (ch == '\n') {
                line[line_it] = 0;
                next = (*callback)(line, user_data);
                line_it = 0;
                continue;
            } else {
                line[line_it] = (char) ch;
            }
            ++line_it;
        }
        free(line);
        fclose(fd);
    }
}

bool parse_line(char *line, void *user_data) {
    struct Property *root = user_data;
    if (line[0] == '#')
        return true;
    char *key = strtok(line, "=");
    char *value = line + strlen(key) + 1;
    property_update(root, strdup(key), strdup(value));
    return true;
}

bool property_dynamic_free(char *key, char *value, void *user_data) {
    if (strcmp(key, "root") == 0)
        return true;
    free(key);
    free(value);
    return true;
}
