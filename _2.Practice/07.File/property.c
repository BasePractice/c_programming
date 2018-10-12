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

static bool property_print(struct Property *element, void *user_data) {
    fprintf(stdout, "%s = %s\n", element->key, element->value);
    return true;
}

void property_add(struct Property *list, char *key, char *value) {
    struct Property *it = list;

    while (it != 0 && it->next != 0) {
        it = it->next;
    }
    if (it != 0) {
        it->next = property_new(key, value);
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